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

#include <unistd.h>
#include "Led.h"

#define D2 13
#define D1 19

int main(int argc, char** argv) {

    try {

        Led d1(D1);
        Led d2(D2);

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

