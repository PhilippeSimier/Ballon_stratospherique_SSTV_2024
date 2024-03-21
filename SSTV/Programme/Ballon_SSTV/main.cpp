/* 
 * File:   main.cpp
 * Author: philippe simier
 *
 * Created on 14 mars 2024, 17:13
 */

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "Camera.h"

using namespace std;

int main(int argc, char** argv) {
    
    int i = 0;
    while (true) {

        // Obtenir l'heure actuelle
        auto maintenant = chrono::system_clock::now();
        auto tempsActuel = chrono::system_clock::to_time_t(maintenant);
        auto tmMaintenant = *localtime(&tempsActuel);

        // Vérifier si c'est un multiple de 8 minutes - 6s
        if (tmMaintenant.tm_sec == 54) {
            if (tmMaintenant.tm_min % 5 == 4) {

                // Appeler la fonction synchronisée
                Camera::envoyerPhoto(29000000);
            } else{
                Camera::enregistrerPhoto(i);
                i++;
                this_thread::sleep_for(chrono::seconds(1));
            }
        } 


    }

    return 0;
}



