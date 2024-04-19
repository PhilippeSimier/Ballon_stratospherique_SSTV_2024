/* 
 * File:   main.cpp
 * Author: philippe SIMIER
 *
 * Created on 14 mars 2024, 17:13
 * ajouter l'option de compilation -pthread lors de la compilation
 * et l'option -lpigpio
 */

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "Camera.h"
#include "bme280.h"
#include "Led.h"

using namespace std;

/**
 *    Fonction exécutée par le deuxième thread
 */
void threadFunction1() {

    
    
    while (true) {

        // Obtenir l'heure actuelle
        auto maintenant = chrono::system_clock::now();
        auto tempsActuel = chrono::system_clock::to_time_t(maintenant);
        auto tmMaintenant = *localtime(&tempsActuel);

        // Vérifier si c'est un multiple de 5 minutes - 6s
        if (tmMaintenant.tm_sec == 54 && tmMaintenant.tm_min % 5 == 4) {

            // Appeler la fonction synchronisée
            
            Camera::envoyerPhoto(29000000);
            
            this_thread::sleep_for(chrono::seconds(1));

        }

    }
}

void threadFunction2() {
    system("/home/ballon/rpi_lora_files/lora_file");
}

int main(int argc, char** argv) {

    BME280 capteur(0x77);
    thread t1(threadFunction1);
    thread t2(threadFunction2);
    Camera camera;
    
/**    Led* ptt = new Led(6);  // Commande Push To Talk
    ptt->setOn();           // active l'ampli en émission
    delete ptt;
**/    
    while (true) {

        // Obtenir l'heure actuelle
        auto maintenant = chrono::system_clock::now();
        auto tempsActuel = chrono::system_clock::to_time_t(maintenant);
        auto tmMaintenant = *localtime(&tempsActuel);

        
        if (tmMaintenant.tm_sec == 30) {
            
            stringstream annotation;
            annotation << fixed << setprecision(1) << capteur.obtenirPression() << " hPa";
            camera.enregistrerPhoto(annotation.str());
            
            this_thread::sleep_for(chrono::seconds(1));

        }
    }
    // Attente de la fin du deuxième thread
    t1.join();
    t2.join();
    

    return 0;

}

