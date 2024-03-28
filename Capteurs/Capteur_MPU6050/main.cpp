/* 
 * File:   main.cpp
 * Author: philippe SIMIER (Lycée Touchard Le Mans)
 *
 * Created on 31 juillet 2023, 17:02
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "MPU6050.h"

using namespace std;



int main(int argc, char** argv) {

    
    try {
        MPU6050 mpu(0x69);
        
        mpu.setAccSensibility(FS_2G);

        while (1) {
            cout << setfill('0') << fixed << setprecision(2) << mpu.getAccelX() << " g ";
            cout << mpu.getAccelY() << " g ";
            cout << mpu.getAccelZ() << " g ";
            cout << setprecision(1) << mpu.getTemperature() << " °C\r" << endl;
            sleep(1);
        }

    } catch (const runtime_error &e) {

        cout << "Exception caught: " << e.what() << endl;
    }
    return 1;
}

