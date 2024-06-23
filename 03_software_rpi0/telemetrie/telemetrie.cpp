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
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

int main(int argc, char **argv)
{
    try
    {
        GestionMesures gestionMesures; // Objet pour la gestion des mesures
        GestionFile gestionFile;       // Objet pour la gestion des files de messages
        GestionTemps gestionTemps;     // Objet pour la gestion du temps

        std::cout << gestionTemps.getDateFormatee();
        std::cout << " : >LOG : Début de la calibration du capteur MPU6050" << std::endl;

        string param = gestionMesures.calibrerMPU();
        std::cout << gestionTemps.getDateFormatee();
        std::cout << " : >LOG : "<< param  << std::endl;

        gestionFile.obtenirFileIPC(5679);  // Obtenir la file pour l'émission key 5679

        // Envoie d'une position pour obtenir un symbole sur la carte aprs.fi
        // le ! indique une position APRS sans timestamp.
        // le _ est le symbole aprs pour une station météo

        std::string payload = "!4800.48N/00011.98E_/A=000268";
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
                gestionMesures.effectuerMesures();
                std::cout << gestionTemps.getDateFormatee();
                std::cout << " : >CSV : ";
                gestionMesures.sauvegarderMesures();

            }


            // Envoyer les mesures par LoRa toutes les 2 minutes à la seconde 30
            if (tmMaintenant.tm_min % 2 == 0 && tmMaintenant.tm_sec == 30)
            {
                gestionMesures.effectuerMesures();
                if (gestionMesures.verifierMesures())
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
