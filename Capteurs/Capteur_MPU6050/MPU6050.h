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

enum AccelSensibility {
	FS_2G = 0x00,     //xxx0 0xxx
	FS_4G = 0x08,     //xxx0 1xxx
	FS_8G = 0x10,     //xxx1 0xxx
	FS_16G = 0x18,    //xxx1 1xxx
	LSB_FS_2G = 16384, 
	LSB_FS_4G = 8192,  
	LSB_FS_8G = 4096,  
	LSB_FS_16G = 2048, 
	ACCEL_MASK = 0xE7 //111x x111
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

private:

    i2c *deviceI2C; // file descriptor

    union data {
        short sData;
        unsigned char uCData[2];
    };

};

#endif /* MPU6050_H */

