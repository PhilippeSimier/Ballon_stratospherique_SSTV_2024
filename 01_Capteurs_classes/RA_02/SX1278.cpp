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
gpio_DIO_0(22),
gpio_reset(0) {
    // Lecture du registre de version 
    int8_t value = spi->read_reg(0x42);
    if (value != 0x12) {
        throw std::runtime_error("Exception in constructor SX1278");
    }
    pinMode(gpio_reset, OUTPUT);

}

SX1278::~SX1278() {
    if (spi != nullptr)
        delete spi;
}

void SX1278::begin(double freq) {

    reset();
    set_lora_mode(); // configure la modulation LoRa
    set_explicit_header(); // configure l'entête explicite
    set_errorcrc(CR5); // configure error coding rate sur CR5
    
    set_bandwidth(BW125); // configure la largeur de bande
    set_sf(SF12); // configure le spreading factor
    set_crc_on();
    set_tx_power(OP20, PA_BOOST);
    set_syncw(0x12);
    set_preamble(6);
    set_agc(1);    // On/Off AGC. If AGC is on, LNAGain not used
    //set_lna(G6, 0); // à creuser !!!
    set_ocp(240); // 45 to 240 mA. 0 to turn off protection

    spi->write_reg(REG_FIFO_TX_BASE_ADDR, TX_BASE_ADDR);
    spi->write_reg(REG_FIFO_RX_BASE_ADDR, RX_BASE_ADDR);
    spi->write_reg(REG_DETECT_OPTIMIZE, 0xc3); //LoRa Detection Optimize for SF > 6
    spi->write_reg(REG_DETECTION_THRESHOLD, 0x0a); //DetectionThreshold for SF > 6

    set_freq(freq);
}

void SX1278::send(unsigned char *buf, unsigned char size) {

    if (get_op_mode() != STDBY_MODE) {
        set_standby_mode();
    }
    set_lowdatarateoptimize_on();

    spi->write_reg(REG_FIFO_ADDR_PTR, TX_BASE_ADDR);
    spi->write_reg(REG_PAYLOAD_LENGTH, size);
    spi->write_fifo(REG_FIFO, buf, size);
    
    set_tx_mode();

}

unsigned char SX1278::get_op_mode() {

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

    spi->write_fifo(REG_PREAMBLE_MSB, (unsigned char *) &len_revers, 2);
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
    spi->write_fifo(REG_FR_MSB, (unsigned char *) &frf_revers, 3);

}

void SX1278::set_lowdatarateoptimize_off() {

    int8_t value = spi->read_reg(REG_MODEM_CONFIG_3) & 0xf7;
    spi->write_reg(REG_MODEM_CONFIG_3, value);
}

void SX1278::set_lowdatarateoptimize_on() {

    int8_t value = spi->read_reg(REG_MODEM_CONFIG_3) & 0xf7;
    spi->write_reg(REG_MODEM_CONFIG_3, value | (0x01 << 3));
}

void SX1278::lora_write_fifo(unsigned char *buf, unsigned char size) {

    spi->write_reg(REG_FIFO_ADDR_PTR, TX_BASE_ADDR);
    spi->write_reg(REG_PAYLOAD_LENGTH, size);
    spi->write_fifo(REG_FIFO, buf, size);

}