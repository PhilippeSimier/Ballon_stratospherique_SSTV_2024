/*
 * File:   main.cpp
 * Author: dbrochard
 *
 * Created on 27 mars 2024, 15:01
 */

#include <iostream>
#include <chrono>
#include <thread>
#include "GestionMesures.h"

int main(int argc, char **argv) {
    std::cout << "[01/04]    | Le programme a bien démarré.\n\rDétection des capteurs..." << std::endl;


    GestionMesures gestionMesures;

    try {

        std::cout << "[03/04]    | Démarrage des mesures..." << std::endl;

        do {
            std::cout
                    << "[04/04]    | Nouvelle mesure  :\n-------------------" << std::endl;

            gestionMesures.effectuerMesures();

            std::cout << "[04/04]    | (MPU 6050) | Acc Z    : " << std::setfill('0') << std::fixed << std::setprecision(2) << gestionMesures.getAccelerationVerticale() << " g " << std::endl;
            std::cout << "[04/04]    | (MPU 6050) | Temp     : " << std::setprecision(1) << gestionMesures.getTemperatureMpu() << " °C" << std::endl;

            std::cout << "[04/04]    | (LM 75)    | Temp     : " << std::setprecision(1) << gestionMesures.getTemperatureLm() << " °C" << std::endl;

            std::cout << "[04/04]    | (BME 280)  | Temp     : " << std::fixed << std::setprecision(1) << gestionMesures.getTemperatureBme() << " °C" << std::endl;
            std::cout << "[04/04]    | (BME 280)  | Pression : " << std::fixed << std::setprecision(1) << gestionMesures.getPression() << " hPa" << std::endl;
            std::cout << "[04/04]    | (BME 280)  | Humidité : " << std::fixed << std::setprecision(1) << gestionMesures.getHumidite() << " %" << std::endl;

            std::cout << "[04/04]    | Fin de la mesure.\n[04/04]    | Vérification des mesures..." << std::endl;

            gestionMesures.verifierMesures();

            std::cout << "[04/04]    | Sauvegarde dans le fichier CSV..." << std::endl;
            gestionMesures.sauvegarderMesures();
            std::cout << "[04/04]    | Sauvegarde dans le fichier CSV achevée avec succès." << std::endl;


            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        } while (1);

    } catch (const runtime_error &e) {
        cout << "Exception caught: " << e.what() << endl;
    }
    return 1;
}
