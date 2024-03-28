/* 
 * File:   MPU6050.h
 * Author: philippe SIMIER (Lycée Touchard Le Mans)
 * 
 * Classe pour un capteur MPU 6050 révision D
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

/*
 * Attention coefficients pour un MPU6050 révision D
 * Divisé par deux pour révision E
 */
enum AccelSensibility {
	FS_2G = 0x00,      //xxx0 0xxx
	FS_4G = 0x08,      //xxx0 1xxx
	FS_8G = 0x10,      //xxx1 0xxx
	FS_16G = 0x18,     //xxx1 1xxx
	LSB_FS_2G = 16384, // 8192 pour un MPU6050 révision E
	LSB_FS_4G = 8192,  // 4096 pour un MPU6050 révision E
	LSB_FS_8G = 4096,  // 2048 pour un MPU6050 révision E
	LSB_FS_16G = 2048, // 1024 pour un MPU6050 révision E
	ACCEL_MASK = 0x18 //0001 1000
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
    
    void setAccSensibility(AccelSensibility range);
    

private:

    i2c *deviceI2C; // file descriptor
    char sensibility; 

    union data {
        short sData;
        unsigned char uCData[2];
    };

};

#endif /* MPU6050_H */

