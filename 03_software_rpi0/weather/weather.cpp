/*
 * File:   telemetrie.cpp
 * Authors: Damien Brochard, Philippe Simier
 *
 * Created on 27 mars 2024, 15:01
 *
 *
 *  pour compiler utiliser le fichier Makefile
 *  make all
 */

#include "GestionMesures.h"
#include "GestionTemps.h"
#include "GestionFile.h"
#include "Position.h"
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

void callback_FF(void);
void callback_ZM(void);
GestionFile gestionFile;       // Objet pour la gestion des files de messages

int main(int argc, char **argv)
{
    try
    {
        GestionMesures gestionMesures; // Objet pour la gestion des mesures

        GestionTemps gestionTemps;     // Objet pour la gestion du temps

        mpu.onFreeFall(callback_FF); // Register a user callback function
        mpu.enableFreeFall(0x80, 10); // seuil (FF très sensible) 0x80 durée 10 ms
        mpu.onZeroMotion(callback_ZM); // Register a user callback function
        mpu.enableZeroMotion(0x05, 10); // seuil (05 très sensible) durée 10 ms

        gestionFile.obtenirFileIPC(5679);  // Obtenir la file pour l'émission key 5679

        // Envoie d'une position pour obtenir une balise sur la carte aprs
        Position pos(47.98321, 0.21060, "Ballon Test", '/', 'O'); // icon ballon
        pos.setAltitude(81.0);
        std::string payload = pos.getPduAprs(true);
        gestionFile.ecrireDansLaFileIPC(payload);
        std::cout << gestionTemps.getDateFormatee();
        std::cout << " : >APLT : ";
        std::cout << payload  << std::endl;

        while (true)
        {
            std::tm tmMaintenant = gestionTemps.obtenirHeureActuelle();

            // Sauvegarder les mesures toutes les 10 secondes
            if (tmMaintenant.tm_sec % 10 == 0)
            {
                gestionMesures.sauvegarderMesures();
            }


            // Envoyer les mesures par LoRa toutes les 2 minutes à la seconde 30
            if (tmMaintenant.tm_min % 2 == 0 && tmMaintenant.tm_sec == 30)
            {

                std::string payload = gestionMesures.formaterMesuresPourLora(); // Formater payload au format lisible par APRS.fi (station WX)
                if (!gestionFile.ecrireDansLaFileIPC(payload))
                {
                    std::cout << gestionTemps.getDateFormatee();
                    std::cout << " : >APLT : Erreur ecriture file" << std::endl;
                }
                else
                {
                    std::cout << gestionTemps.getDateFormatee();
                    std::cout << " : >APLT : ";
                    std::cout << payload  << std::endl;
                }

            }



            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Attendre 1000 ms
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception attrapée: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}

void callback_FF(void) {
    cout << ":f4kmn    :Free fall detected " << endl;
    gestionFile.ecrireDansLaFileIPC(":f4kmn    :Free fall detected ");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Attendre 1000 ms
}

void callback_ZM(void) {
    cout << ":f4kmn    :Zero Motion detected " << endl;
    gestionFile.ecrireDansLaFileIPC(":f4kmn    :Zero Motion detected ");
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Attendre 1000 ms
}
