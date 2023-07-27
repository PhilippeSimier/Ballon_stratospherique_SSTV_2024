/* 
 * File:   main.cpp
 * Author: philippe SIMIER
 *
 * Created on 25 juillet 2023, 15:54
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "LM75.h"
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {

    LM75 capteur;

    while (1) {
        cout << setprecision(3) << capteur.getTemperature() << endl;
        sleep(1);
    }


    return 0;
}

