/* 
 * File:   MPU6050.cpp
 * Author: philippe SIMIER (Lycée Touchard Le Mans)
 * 
 * Created on 31 juillet 2023, 17:06
 */

#include "MPU6050.h"

MPU6050::MPU6050(int8_t address) :
deviceI2C(new i2c(address)),
    ax_offset(0),
    ay_offset(0),
    az_offset(0)
{

    // Configuration du registre d'alimentation
    deviceI2C->WriteReg8(PWR_MGMT_1, 0x00);

    // Lecture du registre d'identification
    unsigned char id = deviceI2C->ReadReg8(WHO_AM_I);


    if (id != 0x68) {
        throw std::runtime_error("Exception in constructor MPU5060");
    }

    // lecture de la sensibilité dans le  registre de configuration
    sensibility = deviceI2C->ReadReg8(ACCEL_CONFIG) & ACCEL_MASK;


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
    temp.uCData[1] = deviceI2C->ReadReg8(TEMP_OUT_H);
    temp.uCData[0] = deviceI2C->ReadReg8(TEMP_OUT_L);

    return (float) temp.sData / 340.0 + 36.53;
}

/**
 * 
 * @return float Accélération axe Z en g
 */
float MPU6050::getAccelZ() {


    data dataAccel;
    dataAccel.uCData[1] = deviceI2C->ReadReg8(ACCEL_ZOUT_H);
    dataAccel.uCData[0] = deviceI2C->ReadReg8(ACCEL_ZOUT_L);
    float val {0.0};
    
    dataAccel.sData -= az_offset;

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
    dataAccel.uCData[1] = deviceI2C->ReadReg8(ACCEL_YOUT_H);
    dataAccel.uCData[0] = deviceI2C->ReadReg8(ACCEL_YOUT_L);

    dataAccel.sData -= ay_offset;

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
    dataAccel.uCData[1] = deviceI2C->ReadReg8(ACCEL_XOUT_H);
    dataAccel.uCData[0] = deviceI2C->ReadReg8(ACCEL_XOUT_L);

    dataAccel.sData -= ax_offset;

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
    
    char val0 = deviceI2C->ReadReg8(ACCEL_CONFIG) & ~ACCEL_MASK;
    deviceI2C->WriteReg8(ACCEL_CONFIG , val0 | range);
    sensibility = deviceI2C->ReadReg8(ACCEL_CONFIG) & ACCEL_MASK;
}

void MPU6050::setAccelXOffset(short offset)
{
    data dataOffset;
    dataOffset.sData = offset;
    deviceI2C->WriteReg8(XA_OFFS_H, dataOffset.uCData[1]);
    deviceI2C->WriteReg8(XA_OFFS_L, dataOffset.uCData[0]);
}

void MPU6050::setAccelYOffset(short offset)
{
    data dataOffset;
    dataOffset.sData = offset;
    deviceI2C->WriteReg8(YA_OFFS_H, dataOffset.uCData[1]);
    deviceI2C->WriteReg8(YA_OFFS_L, dataOffset.uCData[0]);

}

void MPU6050::setAccelZOffset(short offset)
{
    data dataOffset;
    dataOffset.sData = offset;
    deviceI2C->WriteReg8(ZA_OFFS_H, dataOffset.uCData[1]);
    deviceI2C->WriteReg8(ZA_OFFS_L, dataOffset.uCData[0]);

}

/**
 * @brief MPU6050::calibrate
 * @abstract calcule la valeur des offsets
 *           Assumer que l'axe Z pointe vers le haut (+1g)
 *           Lors de la calibration, les valeurs de sortie du capteur sont comparées à des valeurs de référence connues.
 *           Les différences observées sont utilisées pour calculer les valeurs d'offset
 *           qui sont ensuite stockées dans les registres correspondants.
 *           Cette étape est essentielle pour assurer que le capteur fournit des mesures précises et cohérentes.
 */
std::string MPU6050::calibrate(){

    std::ostringstream out;

    int numReadings = 1000;
    long ax_sum = 0, ay_sum = 0, az_sum = 0;
    data dataAccel;

    setAccSensibility(FS_2G);

    // reset offsets
    setAccelXOffset(0);
    setAccelYOffset(0);
    setAccelZOffset(0);

    for (int i = 0; i < numReadings; i++) {

        dataAccel.uCData[1] = deviceI2C->ReadReg8(ACCEL_ZOUT_H);
        dataAccel.uCData[0] = deviceI2C->ReadReg8(ACCEL_ZOUT_L);
        az_sum += dataAccel.sData;

        dataAccel.uCData[1] = deviceI2C->ReadReg8(ACCEL_YOUT_H);
        dataAccel.uCData[0] = deviceI2C->ReadReg8(ACCEL_YOUT_L);
        ay_sum += dataAccel.sData;

        dataAccel.uCData[1] = deviceI2C->ReadReg8(ACCEL_XOUT_H);
        dataAccel.uCData[0] = deviceI2C->ReadReg8(ACCEL_XOUT_L);
        ax_sum += dataAccel.sData;

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Attendre 10 ms

    }

    ax_offset = ax_sum / numReadings;
    ay_offset = ay_sum / numReadings;
    az_offset = az_sum / numReadings - 16384;

    out << " offset AX : " << ax_offset;
    out << " offset AY : " << ay_offset;
    out << " offset AZ : " << az_offset;

    return out.str();

}

