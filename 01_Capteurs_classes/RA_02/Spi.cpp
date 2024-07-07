/* 
 * File:   Spi.cpp
 * Author: philippe SIMIER Lycée Touchard Le Mans
 * 
 * Created on 7 juillet 2024, 16:29
 * Une classe SPI pour accéder aux registres
 * et aux fifo du RA-02  
 * 
 */

#include "Spi.h"

Spi::Spi() :
channel(0),
speed(32000) {

    if (wiringPiSetupGpio() == -1) {
        throw std::runtime_error("Exception Spi wiringPiSetupGpio");
    }
    
    if ((channel  = wiringPiSPISetup(channel, speed)) < 0) {
        throw std::runtime_error("Exception Spi wiringPiSPISetup");
    }
}

Spi::Spi(const Spi& orig) {
}

Spi::~Spi() {
}

/**
 * @brief méthode pour lire un registre
 * @param reg l'adresse du registre
 * @return la valeur lue dans le registre
 */
unsigned char Spi::read_reg(unsigned char reg){
    
    int ret;
    unsigned char data[2];

    data[0] = reg;
    data[1] = 0x00;

    ret = wiringPiSPIDataRW( channel, data, 2);
    if (ret == -1)
        throw std::runtime_error("Exception Spi read_byte");

    return data[1];
}

/**
 * 
 * @param reg l'adresse du registre
 * @param byte
 * @return 
 */
int Spi::write_reg(unsigned char reg, unsigned char value){
    
    int ret;
    unsigned char data[2];
    
    data[0] = (reg | 0x80);    // Bit whr positionné à 1 pour accés en écriture
    data[1] = value;

    ret = wiringPiSPIDataRW( channel, data, 2);
    return ret;    
}

/**
 * 
 * @param reg   adresse de base du fifo
 * @param buff  un pointeur vers un buffer 
 * @param size  la taille des données 
 * @return      le nombre d'octets lus
 */
int Spi::read_fifo(unsigned char reg, unsigned char *buff, unsigned char size){
    
    int ret;
    char unsigned data[257]= {0};

    memset(buff, '\0', size);
    data[0] = reg;
    ret = wiringPiSPIDataRW( channel, data, size+1);
    memcpy(buff, &data[1], ret-1);
    
    return ret; 
}

/**
 * 
 * @param reg addresse de base du fifo
 * @param buff un pointeur sur un buffer
 * @param size la taille des datas à écrire
 * @return le nombre de d'octets écrits
 */
int Spi::write_fifo(unsigned char reg, unsigned char *buff, unsigned char size){
    
    int ret;
    char unsigned data[257]= {0};
    
    data[0]=(reg | 0x80);
    memcpy(&data[1], buff, size);
    ret = wiringPiSPIDataRW( channel, data, size+1);
    

    return ret;
    
}


