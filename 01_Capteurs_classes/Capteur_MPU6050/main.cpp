/*
 * File:   main.cpp
 * Author: philippe SIMIER (Lycée Touchard Washington Le Mans)
 *
 * Created on 31 juillet 2024, 17:02
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <chrono>
#include "MPU6050.h"

using namespace std;

void callback_ZM(void);
void callback_FF(void);


int main(int argc, char** argv) {

    int16_t ox, oy, oz;

    try {

        mpu.begin(0x69);

        mpu.calibrateA();                      // calibration du capteur Acc
        mpu.getAccelOffset(ox, oy, oz);
        cout << "offset Acc: " << ox << " , " << oy << " , " << oz << "\r\n" << endl;
        mpu.calibrateG();                      // calibration du capteur Gyro
        mpu.getGyroOffset(ox,oy,oz);
        cout << "offset Gyro: " << ox << " , " << oy << " , " << oz << "\r\n" << endl;
        sleep(2);

        mpu.setDLPFMode(MPU6050::DLPF_5);     // Filtrage passe bas 5Hz
        mpu.onFreeFall(callback_FF);          // Register a user callback function
        mpu.onZeroMotion(callback_ZM);        // Register a user callback function
        mpu.enableFreeFall(0x80, 1);          // seuil (FF très sensible) 0x80 durée 1 ms
        mpu.enableZeroMotion(0x05, 1);        // seuil (10 très sensible) durée 1 ms
        mpu.setAccSensibility(MPU6050::FS_4G);
        mpu.setGyroSensibility(MPU6050::FS_1000DPS);

        cout << setfill('0') << fixed << setprecision(2) << std::showpos;

        while (1) {

            cout << "Accel:  " << mpu.getAccelX() << "\t" << mpu.getAccelY() << "\t" << mpu.getAccelZ() << " g\t";
            cout << "Gyro:  " << mpu.getRotationX() << "\t" << mpu.getRotationY() << "\t" << mpu.getRotationZ() << " dps\t";
            cout << "Temp:   " << mpu.getTemperature() << " °C" << endl;
            sleep(1);
        }

    } catch (const runtime_error &e) {

        cout << "Exception caught: " << e.what() << endl;
    }
    return 0;
}

void callback_ZM(void) {
    cout << " Zero motion detected " << endl;
}

void callback_FF(void) {
    cout << " Free fall detected " << endl;
}



