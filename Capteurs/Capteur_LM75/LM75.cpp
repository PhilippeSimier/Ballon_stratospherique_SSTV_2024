/* 
 * File:   LM75.cpp
 * Author: philippe SIMIER
 * 
 * Created on 25 juillet 2023, 15:55
 */

#include "LM75.h"


LM75::LM75(int8_t address) {

    fd = wiringPiI2CSetup(address);
    
}

LM75::LM75(const LM75& orig) {
}

LM75::~LM75() {
    close(fd);
}

/**
 * @brief  methode pour obtenir la température
 * @return float la valeur de la température en °C
 */
float LM75::getTemperature() {
    
    union {
        int val;
        char octet[4];
    }reg0;

    reg0.val = wiringPiI2CReadReg16(fd, 0x00);
       
    float temp = ((int16_t)((reg0.octet[0]<<8) | reg0.octet[1])) / 256.0 ;
    
    return temp;

}