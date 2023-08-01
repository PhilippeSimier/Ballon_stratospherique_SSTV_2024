/* 
 * File:   main.cpp
 * Author: philippe
 * 
 * Programe pour tester les leds GPIO13 et GPIO19
 * 
 * ce programme fonctionne avec les privilèges de root
 * installer la bibliothèque pigpio
 * apt install pigpio
 *
 * Created on 1 août 2023, 21:40
 */

#include <cstdlib>

using namespace std;

#include <pigpio.h>
#include <unistd.h>

#define D2 13

int main(int argc, char** argv) {

    if (gpioInitialise() < 0) {
        return -1;
    }

    gpioSetMode(D2, PI_OUTPUT);

    for (int compteur = 0; compteur < 60; compteur++) {
        gpioWrite(D2, 1);
        sleep(1);
        gpioWrite(D2, 0);
        sleep(1);
    }
    gpioTerminate();


    return 0;
}

