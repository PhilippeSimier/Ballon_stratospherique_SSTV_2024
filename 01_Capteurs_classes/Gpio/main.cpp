/* 
 * File:   main.cpp
 * Author: philippe SIMIER Lycée Touchard
 * 
 * Programe pour tester les leds GPIO13 et GPIO19
 * 
 *
 * Created on 1 août 2023, 21:40
 */
#include <cstdlib>
#include <unistd.h>
#include "GpioOut.h"

using namespace std;

#define D2 13
#define D1 19

int main(int argc, char** argv) {

    try {

        GpioOut d1(D1);
        GpioOut d2(D2);

        for (int compteur = 0; compteur < 60; compteur++) {

            d1.setOn();
            d2.setOff();
            sleep(1);

            d1.setOff();
            d2.setOn();
            sleep(1);
        }
        
    } catch (const runtime_error &e) {

        cout << "Exception caught: " << e.what() << endl;
        return 1;
    }

    return 0;
}

