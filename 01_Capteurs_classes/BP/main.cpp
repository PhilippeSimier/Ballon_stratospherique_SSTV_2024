/* 
 * File:   main.cpp
 * Author: philippe SIMIER
 * 
 * Un programme tout simple pour tester le bouton poussoir
 *
 * Created on 2 août 2023, 17:40
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <pigpio.h>

using namespace std;

#define BP 25

int main(int argc, char** argv) {

    gpioInitialise();
    gpioSetMode(BP, PI_INPUT);
    int compteur {0};

    while (compteur < 10) {
        if (gpioRead(BP) == 0) {
            
            compteur++;
            cout << "BP est actionné " << compteur << "\r" << endl;
            sleep(1);
        }
    }

    gpioTerminate();

    return 0;
}

