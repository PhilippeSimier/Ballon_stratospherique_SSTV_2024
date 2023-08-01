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
    deviceI2C->WriteReg8(0x6B, 0x00);

    // Lecture du registre d'identification
    unsigned char id = deviceI2C->ReadReg8(0x75);




    if (id != address) {
        throw std::runtime_error("Exception in constructor MPU5060");
    }

    // lecture de la sensibilité dans le  registre de configuration
    sensibility = deviceI2C->ReadReg8(0x1C) & ACCEL_MASK;


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

    return (float) temp.sData / 340.0 + 36.53;
}

/**
 * 
 * @return float Accélération axe Z en g
 */
float MPU6050::getAccelZ() {


    data dataAccel;
    dataAccel.uCData[1] = deviceI2C->ReadReg8(0x3f);
    dataAccel.uCData[0] = deviceI2C->ReadReg8(0x40);
    float val {0.0};
    
    switch (sensibility) {
        case FS_2G:
            val = dataAccel.sData / (float) LSB_FS_2G;
            break;
        case FS_4G:
            val = dataAccel.sData / (float) LSB_FS_4G;
            break;
        case FS_8G:
            val = dataAccel.sData / (float) LSB_FS_8G;
            break;
        case FS_16G:
            val = dataAccel.sData / (float) LSB_FS_16G;
            break;
    }
    return val;
}

/**
 * 
 * @return float Accélération axe Y en g
 */

float MPU6050::getAccelY() {

    data dataAccel;
    dataAccel.uCData[1] = deviceI2C->ReadReg8(0x3d);
    dataAccel.uCData[0] = deviceI2C->ReadReg8(0x3e);

    float val {0.0};
    
    switch (sensibility) {
        case FS_2G:
            val = dataAccel.sData / (float) LSB_FS_2G;
            break;
        case FS_4G:
            val = dataAccel.sData / (float) LSB_FS_4G;
            break;
        case FS_8G:
            val = dataAccel.sData / (float) LSB_FS_8G;
            break;
        case FS_16G:
            val = dataAccel.sData / (float) LSB_FS_16G;
            break;
    }
    return val; 


}

/**
 * 
 * @return float Accélération axe X en g
 */
float MPU6050::getAccelX() {

    data dataAccel;
    dataAccel.uCData[1] = deviceI2C->ReadReg8(0x3b);
    dataAccel.uCData[0] = deviceI2C->ReadReg8(0x3c);

    float val {0.0};
    
    switch (sensibility) {
        case FS_2G:
            val = dataAccel.sData / (float) LSB_FS_2G;
            break;
        case FS_4G:
            val = dataAccel.sData / (float) LSB_FS_4G;
            break;
        case FS_8G:
            val = dataAccel.sData / (float) LSB_FS_8G;
            break;
        case FS_16G:
            val = dataAccel.sData / (float) LSB_FS_16G;
            break;
    }
    return val;
}

/**
 * @brief methode pour configurer la sensibilité de l'accélérometre.
 * @param range FS_2G ou FS_4G ou FS_8G ou FS_16G
 */
void MPU6050::setAccSensibility(AccelSensibility range){
    
    char val0 = deviceI2C->ReadReg8(0x1C) & ~ACCEL_MASK;
    deviceI2C->WriteReg8(0x1C , val0 | range);
    sensibility = deviceI2C->ReadReg8(0x1C) & ACCEL_MASK;
}

