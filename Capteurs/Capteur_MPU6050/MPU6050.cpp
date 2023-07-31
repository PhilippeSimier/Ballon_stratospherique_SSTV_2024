/* 
 * File:   MPU6050.cpp
 * Author: philippe SIMIER (Lycée Touchard Le Mans)
 * 
 * Created on 31 juillet 2023, 17:06
 */

#include "MPU6050.h"

MPU6050::MPU6050(int8_t address) :
deviceI2C(new i2c(address)) {
    
    // Configuration du registre d'alimentation
    deviceI2C->WriteReg8 (0x6B, 0x00);
    
    // Lecture du registre d'identification
    unsigned char id = deviceI2C->ReadReg8(0x75);
    
    if (id != address){
        throw std::runtime_error("Exception in constructor MPU5060");
    }
    


}

MPU6050::MPU6050(const MPU6050& orig) {
}

MPU6050::~MPU6050() {
    if (deviceI2C != nullptr)
        delete deviceI2C;
}
/**
 * 
 * @return float la température en degré celcius
 */
float MPU6050::getTemperature() {
    
    data temp;
    temp.uCData[1] = deviceI2C->ReadReg8(0x41);
    temp.uCData[0] = deviceI2C->ReadReg8(0x42);
    
    return (float)temp.sData / 340.0 + 36.53;
}

float MPU6050::getAccelZ() {
    
    data dataAccel;
    dataAccel.uCData[1] = deviceI2C->ReadReg8(0x3f);
    dataAccel.uCData[0] = deviceI2C->ReadReg8(0x40);
    
    return dataAccel.sData/(float)LSB_FS_2G;
    
    
}

float MPU6050::getAccelY() {
    
    data dataAccel;
    dataAccel.uCData[1] = deviceI2C->ReadReg8(0x3d);
    dataAccel.uCData[0] = deviceI2C->ReadReg8(0x3e);
    
    return dataAccel.sData/(float)LSB_FS_2G;
    
    
}

float MPU6050::getAccelX() {
    
    data dataAccel;
    dataAccel.uCData[1] = deviceI2C->ReadReg8(0x3b);
    dataAccel.uCData[0] = deviceI2C->ReadReg8(0x3c);
    
    return dataAccel.sData/(float)LSB_FS_2G;
    
    
}

