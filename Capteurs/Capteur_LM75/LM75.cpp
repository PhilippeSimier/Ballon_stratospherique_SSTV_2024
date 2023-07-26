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

    int reg0 = wiringPiI2CReadReg16(fd, 0x00);

    int temp0 = (int8_t) (reg0 & 0x00ff);
    int temp1 = (reg0 & 0x8000) >> 15;
       
    float temp = temp0 + 0.5 * temp1;
    
    return temp;

}