/* 
 * File:   SX1278.cpp
 * Author: philippe SIMIER Lycée Touchard Le Mans
 * 
 * Created on 8 juillet 2024, 08:32
 */

#include "SX1278.h"

/**
 * @brief Ouvre le bus SPI et 
 *        vérifie que le SX1278 est présent
 * @param channel le canal du bus
 */
SX1278::SX1278(int channel) :
spi(new Spi(channel)),
gpio_reset(0),
gpio_DIO_0(22),
bw(BW125),
sf(SF12),
ecr(CR5),
freq(433775000),
preambleLen(6),
syncWord(0x12),
outPower(OP20),
powerOutPin(PA_BOOST),
ocp(240) {
    // Lecture du registre de version 
    int8_t value = spi->read_reg(0x42);
    if (value != 0x12) {
        throw std::runtime_error("Exception in constructor SX1278");
    }
    pinMode(gpio_reset, OUTPUT);

    pinMode(gpio_DIO_0, INPUT);
    // appelle de la fonction ISR_Function sur interruption au front montant de DIO_0
    if (wiringPiISR(gpio_DIO_0, INT_EDGE_RISING, &ISR_Function) < 0) {
        throw std::runtime_error("Exception in constructor DIO_0");
    }

}

SX1278::~SX1278() {
    if (spi != nullptr)
        delete spi;
}

void SX1278::begin() {

    reset();
    set_lora_mode(); // configure la modulation mode LoRa
    set_explicit_header(); // configure l'entête explicite
    set_errorcrc(ecr); // configure error coding rate

    set_bandwidth(bw); // configure la largeur de bande
    set_sf(sf); // configure le spreading factor
    set_crc_on();
    set_tx_power(outPower, powerOutPin);
    set_syncw(syncWord);
    set_preamble(preambleLen);
    set_agc(1); // On/Off AGC. If AGC is on, LNAGain not used
    //set_lna(G6, 0); // à creuser !!!
    set_ocp(ocp); // 45 to 240 mA. 0 to turn off protection

    spi->write_reg(REG_FIFO_TX_BASE_ADDR, TX_BASE_ADDR);
    spi->write_reg(REG_FIFO_RX_BASE_ADDR, RX_BASE_ADDR);
    spi->write_reg(REG_DETECT_OPTIMIZE, 0xc3); //LoRa Detection Optimize for SF > 6
    spi->write_reg(REG_DETECTION_THRESHOLD, 0x0a); //DetectionThreshold for SF > 6

    set_freq(freq);
}

void SX1278::send(int8_t *buf, int8_t size) {

    if (get_op_mode() != STDBY_MODE) {
        set_standby_mode();
    }
    calculate_packet_t(size); // Calcule le temps pour émettre le packet 

    spi->write_reg(REG_FIFO_ADDR_PTR, TX_BASE_ADDR);
    spi->write_reg(REG_PAYLOAD_LENGTH, size);
    spi->write_fifo(REG_FIFO, buf, size);

    set_dio_tx_mapping();
    set_tx_mode();

}

void SX1278::send(const std::string &message) {


    auto longueur = message.length();
    if (longueur > 255) {
        throw std::runtime_error("Exception in send(char*) SX1278");
    }
    send((int8_t *) message.c_str(), (int8_t) longueur);
}

void SX1278::receive() {

    calculate_packet_t(0);
    if (get_op_mode() != STDBY_MODE) {
        set_standby_mode();
    }

    set_dio_rx_mapping();
    set_rxcont_mode();
}

int8_t SX1278::get_op_mode() {

    return (spi->read_reg(REG_OP_MODE) & 0x07);

}

void SX1278::set_standby_mode() {

    int8_t value = spi->read_reg(REG_OP_MODE) & 0xf8;
    spi->write_reg(REG_OP_MODE, value | STDBY_MODE);

}

void SX1278::set_tx_mode() {

    int8_t value = spi->read_reg(REG_OP_MODE) & 0xf8;
    spi->write_reg(REG_OP_MODE, value | TX_MODE);

}

void SX1278::set_rxcont_mode() {

    int8_t value = spi->read_reg(REG_OP_MODE) & 0xf8;
    spi->write_reg(REG_OP_MODE, value | RXCONT_MODE);

}

void SX1278::reset() {

    digitalWrite(gpio_reset, LOW);
    usleep(100);
    digitalWrite(gpio_reset, HIGH);
    usleep(5000);
}

void SX1278::set_lora_mode() {
    set_sleep_mode();
    int8_t value = spi->read_reg(REG_OP_MODE) & 0x7f;
    spi->write_reg(REG_OP_MODE, value | LORA_MODE);

}

void SX1278::set_sleep_mode() {
    int8_t value = spi->read_reg(REG_OP_MODE) & 0xf8;
    spi->write_reg(REG_OP_MODE, value | SLEEP_MODE);
}

void SX1278::set_explicit_header() {
    int8_t value = spi->read_reg(REG_MODEM_CONFIG_1) & 0xfe;
    spi->write_reg(REG_MODEM_CONFIG_1, value);
}

void SX1278::set_errorcrc(ErrorCodingRate cr) {
    int8_t value = spi->read_reg(REG_MODEM_CONFIG_1) & 0xf1;
    spi->write_reg(REG_MODEM_CONFIG_1, value | cr);
}

void SX1278::set_crc_on() {
    int8_t value = spi->read_reg(REG_MODEM_CONFIG_2) & 0xfb;
    spi->write_reg(REG_MODEM_CONFIG_2, value | 0x01 << 2);
}

void SX1278::set_crc_off() {
    int8_t value = spi->read_reg(REG_MODEM_CONFIG_2) & 0xfb;
    spi->write_reg(REG_MODEM_CONFIG_2, value);
}

void SX1278::set_bandwidth(BandWidth bw) {
    int8_t value = spi->read_reg(REG_MODEM_CONFIG_1) & 0x0f;
    spi->write_reg(REG_MODEM_CONFIG_1, value | bw);
}

void SX1278::set_sf(SpreadingFactor sf) {
    int8_t value = spi->read_reg(REG_MODEM_CONFIG_2) & 0x0f;
    spi->write_reg(REG_MODEM_CONFIG_2, value | sf);

}

void SX1278::set_tx_power(OutputPower power, PowerAmplifireOutputPin pa_pin) {

    if (pa_pin == RFO) {
        power = power >= OP15 ? OP15 : (power <= OP0 ? OP0 : power);
        spi->write_reg(REG_PA_DAC, 0x84); //default val to +17dBm
        spi->write_reg(REG_PA_CONFIG, pa_pin | power);
        return;
    } else if (pa_pin == PA_BOOST) {
        if (power == OP20) {
            spi->write_reg(REG_PA_DAC, 0x87); //Max 20dBm power
            spi->write_reg(REG_PA_CONFIG, pa_pin | (power - 2));
            return;
        } else {
            power = power >= OP17 ? OP17 : (power <= OP2 ? OP2 : power);
            spi->write_reg(REG_PA_DAC, 0x84); //default val to +17dBm
            spi->write_reg(REG_PA_CONFIG, pa_pin | (power - 2));
            return;
        }
    }
}

void SX1278::set_syncw(unsigned char word) {

    spi->write_reg(REG_SYNC_WORD, word);

}

void SX1278::set_preamble(int preambleLen) {

    if (preambleLen < 6) {
        preambleLen = 6;
    } else if (preambleLen > 65535) {
        preambleLen = 65535;
    }
    unsigned short len_revers = 0;
    len_revers += ((unsigned char) (preambleLen >> 0)) << 8;
    len_revers += ((unsigned char) (preambleLen >> 8)) << 0;

    spi->write_fifo(REG_PREAMBLE_MSB, (int8_t *) & len_revers, 2);
}

void SX1278::set_agc(_Bool AGC) {

    spi->write_reg(REG_MODEM_CONFIG_3, (AGC << 2));

}

void SX1278::set_lna(LnaGain lnaGain, _Bool lnaBoost) {

    spi->write_reg(REG_LNA, ((lnaGain << 5) + lnaBoost));

}


// output courent protection

void SX1278::set_ocp(unsigned char OCP) {

    unsigned char OcpTrim;
    if (OCP == 0) { //turn off OCP

        spi->write_reg(REG_OCP, (spi->read_reg(REG_OCP) & 0xdf));

    } else if (OCP > 0 && OCP <= 120) {
        if (OCP < 50) {
            OCP = 50;
        }

        OcpTrim = (OCP - 45) / 5 + 0x20;
        spi->write_reg(REG_OCP, OcpTrim);
    } else if (OCP > 120) {

        if (OCP < 130) {
            OCP = 130;
        }

        OcpTrim = (OCP + 30) / 10 + 0x20;
        spi->write_reg(REG_OCP, OcpTrim);
    }


}

void SX1278::set_freq(double freq) {

    int frf, frf_revers = 0;
    frf = (int) ceil((freq / (32000000.0))*524288);
    frf_revers += (int) ((unsigned char) (frf >> 0)) << 16;
    frf_revers += (int) ((unsigned char) (frf >> 8)) << 8;
    frf_revers += (int) ((unsigned char) (frf >> 16) << 0);
    spi->write_fifo(REG_FR_MSB, (int8_t *) & frf_revers, 3);

}

void SX1278::set_lowdatarateoptimize_off() {

    int8_t value = spi->read_reg(REG_MODEM_CONFIG_3) & 0xf7;
    spi->write_reg(REG_MODEM_CONFIG_3, value);
}

void SX1278::set_lowdatarateoptimize_on() {

    int8_t value = spi->read_reg(REG_MODEM_CONFIG_3) & 0xf7;
    spi->write_reg(REG_MODEM_CONFIG_3, value | (0x01 << 3));
}

void SX1278::lora_write_fifo(int8_t *buf, int8_t size) {

    spi->write_reg(REG_FIFO_ADDR_PTR, TX_BASE_ADDR);
    spi->write_reg(REG_PAYLOAD_LENGTH, size);
    spi->write_fifo(REG_FIFO, buf, size);

}

void SX1278::calculate_packet_t(int8_t payloadLen) {

    unsigned BW_VAL[10] = {7800, 10400, 15600, 20800, 31250, 41700, 62500, 125000, 250000, 500000};

    unsigned bw_val = BW_VAL[(bw >> 4)];
    unsigned sf_val = sf >> 4;
    unsigned char ecr_val = 4 + (ecr / 2);

    double Tsym = (pow(2, sf_val) / bw_val)*1000;
    bool lowDataRateOptimize = (Tsym > 16);

    if (lowDataRateOptimize)
        set_lowdatarateoptimize_on();
    else
        set_lowdatarateoptimize_off();

    double Tpreambule = (preambleLen + 4.25) * Tsym;
    int tmpPoly = (8 * payloadLen - 4 * sf_val + 28 + 16);
    if (tmpPoly < 0) {
        tmpPoly = 0;
    }
    unsigned payloadSymbNb = 8 + ceil(((double) tmpPoly) / (4 * (sf_val - 2 * lowDataRateOptimize))) * ecr_val;
    double Tpayload = payloadSymbNb * Tsym;
    double Tpacket = Tpayload + Tpreambule;

    std::cout << "Tpacket : " << Tpacket << std::endl;

}

void SX1278::set_dio_rx_mapping() {
    spi->write_reg(REG_DIO_MAPPING_1, 0 << 6);
}

void SX1278::set_dio_tx_mapping() {
    spi->write_reg(REG_DIO_MAPPING_1, 1 << 6);
}

void SX1278::reset_irq_flags() {
    spi->write_reg(REG_IRQ_FLAGS, 0xff);
}

/**
 * Méthode exécutée suite à une interruption
 */
void SX1278::DoneISRf() {

    if (spi->read_reg(REG_IRQ_FLAGS) & IRQ_RXDONE) { // fin de réception d'un payload
       
        std::cout << "RX done" << std::endl;
        
        int8_t value = spi->read_reg( REG_FIFO_RX_CURRENT_ADDR );
        spi->write_reg( REG_FIFO_ADDR_PTR, value );
        int8_t rx_nb_bytes = spi->read_reg(REG_RX_NB_BYTES);
        spi->read_fifo(REG_FIFO, bufferRX, rx_nb_bytes);
        bufferRX[rx_nb_bytes] = '\0';
        std::cout << bufferRX << std::endl;
    }
    if (spi->read_reg(REG_IRQ_FLAGS) & IRQ_TXDONE) { // fin de l'émission d'un payload
        
        std::cout << "TX done" << std::endl;

        set_standby_mode();
        set_dio_rx_mapping();
        set_rxcont_mode(); // passage en réception continue
    }

    reset_irq_flags();
}

/**
 * Cette fonction est statique 
 * elle  utilise la variable globale loRa pour accéder
 * au contexte 
 */
void SX1278::ISR_Function() {

    extern SX1278 loRa;
    loRa.DoneISRf();
}

SX1278 loRa;

