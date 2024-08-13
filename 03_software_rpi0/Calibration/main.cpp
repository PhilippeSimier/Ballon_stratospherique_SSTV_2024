/* 
 * File:   main.cpp
 * Author: philippe SIMIER  Touchard Washington le Mans
 *
 * Created on 13 août 2024, 15:12
 */

#include <cstdlib>
#include <iostream>
#include "MPU6050.h"
#include "SimpleIni.h"

#define CONFIGURATION "/home/ballon/telemetrie/config_MPU6050.ini"

using namespace std;

int main(int argc, char** argv) {

    int16_t ox, oy, oz;
    try {

        cout << "Programme calibration capteur MPU6050" << endl;

        SimpleIni ini(CONFIGURATION);



        mpu.begin(0x69);
        mpu.calibrateA(); // calibration du capteur Acc
        mpu.getAccelOffset(ox, oy, oz);
        cout << "offset Acc: " << ox << " , " << oy << " , " << oz << "\r\n" << endl;
        ini.SetValue<int>("MPU6050", "offsetAX", ox);
        ini.SetValue<int>("MPU6050", "offsetAY", oy);
        ini.SetValue<int>("MPU6050", "offsetAZ", oz);

        mpu.calibrateG(); // calibration du capteur Gyro
        mpu.getGyroOffset(ox, oy, oz);
        cout << "offset Gyro: " << ox << " , " << oy << " , " << oz << "\r\n" << endl;
        ini.SetValue<int>("MPU6050", "offsetGX", ox);
        ini.SetValue<int>("MPU6050", "offsetGY", oy);
        ini.SetValue<int>("MPU6050", "offsetGZ", oz);
        
        if (ini.Save()){
             cout << "Les offsets ont été enregistrés dans le fichier " << endl;
             cout << CONFIGURATION << endl;
        }



    } catch (const runtime_error &e) {

        cout << "Exception caught: " << e.what() << endl;
    }
    return 0;
}

