/* 
 * File:   MPU6050.h
 * Author: philippe SIMIER (Lycée Touchard Le Mans)
 *
 * Created on 31 juillet 2023, 17:06
 */

#ifndef MPU6050_H
#define MPU6050_H

#include "i2c.h"
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <sstream>

#define EARTH_GRAVITY_0 9.80665

// Il y a 4 calibres pour l'accélération
enum AccelSensibility {
    FS_2G = 0x00,     //xxx0 0xxx  calibre 0 : +/-2g
    FS_4G = 0x08,     //xxx0 1xxx  calibre 1 : +/- 4g
    FS_8G = 0x10,     //xxx1 0xxx  calibre 2 : +/- 8g
    FS_16G = 0x18,    //xxx1 1xxx  calibre 3 : +/- 16g
    LSB_FS_2G = 16384,
    LSB_FS_4G = 8192,
    LSB_FS_8G = 4096,
    LSB_FS_16G = 2048,
    ACCEL_MASK = 0x18 //0001 1000
};

// Le filtre passe-bas numérique intégré dans le MPU6050 peut être configuré avec différentes bandes passantes
// Le prix à payer pour ce filtrage est le léger retard du signal numérique par rapport au signal analogique.
enum Dlpf {
    DLPF_260,  // 260Hz     | 0ms
    DLPF_184,  // 184Hz     | 2.0ms
    DLPF_94,   // 94Hz      | 3.0ms
    DLPF_44,   // 44Hz      | 4.9ms
    DLPF_21,   // 21Hz      | 8.5ms
    DLPF_10,   // 10Hz      | 13.8ms
    DLPF_5,    //  5Hz      | 19.0ms
};

enum Registre {

    XA_OFFS_H = 0x06, //[15:0] XA_OFFS
    XA_OFFS_L = 0x07,
    YA_OFFS_H = 0x08, //[15:0] YA_OFFS
    YA_OFFS_L = 0x09,
    ZA_OFFS_H = 0x0A, //[15:0] ZA_OFFS
    ZA_OFFS_L = 0x0B,

    CONFIG = 0x1A,
    GYRO_CONFIG = 0x1B,
    ACCEL_CONFIG = 0x1C,

    ACCEL_XOUT_H = 0x3B,
    ACCEL_XOUT_L = 0x3C,
    ACCEL_YOUT_H = 0x3D,
    ACCEL_YOUT_L = 0x3E,
    ACCEL_ZOUT_H = 0x3F,
    ACCEL_ZOUT_L = 0x40,
    TEMP_OUT_H = 0x41,
    TEMP_OUT_L = 0x42,
    GYRO_XOUT_H = 0x43,
    GYRO_XOUT_L = 0x44,
    GYRO_YOUT_H = 0x45,
    GYRO_YOUT_L = 0x46,
    GYRO_ZOUT_H = 0x47,
    GYRO_ZOUT_L = 0x48,

    PWR_MGMT_1 = 0x6B,
    WHO_AM_I = 0x75

};

class MPU6050 {
public:

    MPU6050(int8_t address = 0x68);
    MPU6050(const MPU6050& orig);
    virtual ~MPU6050();

    float getTemperature();
    float getAccelX();
    float getAccelY();
    float getAccelZ();
    float getAccelM();
    
    void setAccSensibility(AccelSensibility range);
    void setDLPFMode(Dlpf dlpf);

    void setAccelXOffset(short offset);
    void setAccelYOffset(short offset);
    void setAccelZOffset(short offset);

    std::string calibrate();


private:

    i2c *deviceI2C; // file descriptor
    char sensibility; 

    union data {
        short sData;
        unsigned char uCData[2];
    };

    short ax_offset, ay_offset, az_offset;

};

#endif /* MPU6050_H */
