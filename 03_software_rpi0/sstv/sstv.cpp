/*
 * File:   main.cpp
 * Author: philippe SIMIER (F4JRE)
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
#include <filesystem>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "Camera.h"
#include "GpioOut.h"
#include "SimpleIni.h"

#define CONFIGURATION "/home/ballon/configuration.ini"
using namespace std;

GpioOut ptt(6);  // Commande Push To Talk

int main(int argc, char** argv) {

    if (geteuid() != 0) {
        std::cerr << "Ce programme doit être lancé en tant que root." << std::endl;
        return 1;
    }

    SimpleIni ini;

    ini.Load(CONFIGURATION);
    unsigned long freq = ini.GetValue<unsigned long>("sstv", "freq", 29000000);      // setting freq sstv
    string indicatif = ini.GetValue("aprs", "indicatif", "F4ABC");

    cout << "sstv freq = " << freq << " ssid = " << indicatif << endl;
    Camera camera(freq, indicatif);
    int i = 1;

    while (true) {

        // Obtenir l'heure actuelle
        auto maintenant = chrono::system_clock::now();
        auto tempsActuel = chrono::system_clock::to_time_t(maintenant);
        auto tmMaintenant = *localtime(&tempsActuel);

        if (std::filesystem::exists("/ramfs/mire")) {
            cout << "mire : " << i++ << endl;
            camera.envoyerMire();
            this_thread::sleep_for(chrono::seconds(60));

        }

        if (tmMaintenant.tm_sec == 30) {

            camera.enregistrerPhoto();
            this_thread::sleep_for(chrono::seconds(1));

        }
        if (tmMaintenant.tm_sec == 55 && tmMaintenant.tm_min % 5 == 4) {

            // Appeler la methode envoyer photo en SSTV
            ptt.setOn(); // active l'ampli
            camera.envoyerPhoto();
            this_thread::sleep_for(chrono::seconds(1));
            ptt.setOff(); // désactive l'ampli

        }
    }

    return 0;

}






