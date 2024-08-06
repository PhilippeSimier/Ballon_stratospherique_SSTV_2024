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
        mpu.setAccSensibility(MPU6050::FS_2G);
        mpu.calibrate();                      // calibration du capteur
        mpu.getAccelOffset(ox, oy, oz);
        cout << "offset : " << ox << " , " << oy << " , " << oz << endl;
        
        mpu.setDLPFMode(MPU6050::DLPF_260);   // Filtrage passe bas 260Hz
        mpu.onFreeFall(callback_FF);          // Register a user callback function 
        mpu.onZeroMotion(callback_ZM);        // Register a user callback function 
        mpu.enableFreeFall(0x80, 1);          // seuil (FF très sensible) 0x80 durée 1 ms
        mpu.enableZeroMotion(0x80, 10);       // seuil (10 très sensible) durée 10 ms

        cout << setfill('0') << fixed << setprecision(2);

        while (1) {

            cout << mpu.getAccelX() << " : " << mpu.getAccelY() << " : " << mpu.getAccelZ() << endl;
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



