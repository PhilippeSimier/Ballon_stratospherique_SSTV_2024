/*
 * File:   main.cpp
 * Author: philippe SIMIER
 *
 * Created on 14 mars 2024, 17:13
 * compilation make all
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "Camera.h"
#include "GpioOut.h"

using namespace std;



GpioOut ptt(6);  // Commande Push To Talk

int main(int argc, char** argv) {


    Camera camera;

    while (true) {

        // Obtenir l'heure actuelle
        auto maintenant = chrono::system_clock::now();
        auto tempsActuel = chrono::system_clock::to_time_t(maintenant);
        auto tmMaintenant = *localtime(&tempsActuel);

        if (tmMaintenant.tm_sec == 30) {

            camera.enregistrerPhoto("");

            this_thread::sleep_for(chrono::seconds(1));

        }
        if (tmMaintenant.tm_sec == 54 && tmMaintenant.tm_min % 5 == 4) {

            // Appeler la methode envoyer photo en SSTV
            ptt.setOn(); // active l'ampli
            Camera::envoyerPhoto(29000000);
            this_thread::sleep_for(chrono::seconds(1));
            ptt.setOff(); // désactive l'ampli

        }


    }

    return 0;

}






