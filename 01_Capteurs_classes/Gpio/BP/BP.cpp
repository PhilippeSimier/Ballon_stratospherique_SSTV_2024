/* 
 * File:   main.cpp
 * Author: philippe SIMIER
 * 
 * Un programme tout simple pour tester le bouton poussoir
 *
 *   git clone https://github.com/WiringPi/WiringPi.git
 *   cd WiringPi
 *   ./build
 *
 *   g++ BP.cpp -lwiringPi -o BP
 *
 * Created on 2 août 2023, 17:40
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>

using namespace std;

#define BP 25

int main(int argc, char** argv) {

    int compteur {0};

    wiringPiSetupGpio();
    pinMode(BP, INPUT);

    while (compteur < 10) {
        if (digitalRead(BP) == 0) {
            
            compteur++;
            cout << "BP est actionné " << compteur << "\r" << endl;
            sleep(1);
        }
    }

    return 0;
}

