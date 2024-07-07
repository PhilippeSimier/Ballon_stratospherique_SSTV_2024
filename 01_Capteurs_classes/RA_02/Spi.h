/* 
 * File:   Spi.h
 * Author: philippe SIMIER Lycée Touchard Le Mans
 *
 * Created on 7 juillet 2024, 16:29
 */

#ifndef SPI_H
#define SPI_H

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <wiringPi.h>
#include "wiringPiSPI.h"

class Spi {
    
public:
    
    Spi();
    Spi(const Spi& orig);
    virtual ~Spi();
    
    unsigned char read_reg(unsigned char reg);
    int write_reg(unsigned char reg, unsigned char byte);
    
    int read_fifo( unsigned char reg, unsigned char *buff, unsigned char size);
    int write_fifo(unsigned char reg, unsigned char *buff, unsigned char size);
    
    
    
private:
    
    int channel;
    int speed;
    

};

#endif /* SPI_H */

