/*
 * File:   main.cpp
 * Author: dbrochard
 *
 * Created on 27 mars 2024, 15:01
 * 
 * 
 * 
 * sudo g++ -o ./main ./*.cpp ./*.h -lpthread -lrt && sudo ./main
 */

#include "GestionMesures.h"
#include "GestionTemps.h"
#include "GestionFile.h"

#include <chrono>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>

int main(int argc, char **argv)
{
  try
  {
    GestionMesures gestionMesures; // Objet pour la gestion des mesures
    GestionFile gestionFile;       // Objet pour la gestion des files de messages
    GestionTemps gestionTemps;     // Objet pour la gestion du temps

    // Obtenir l'heure actuelle
    std::tm tmMaintenant = gestionTemps.obtenirHeureActuelle();
    std::tm tmPrecedent = gestionTemps.obtenirHeureActuelle();
    tmPrecedent.tm_sec = tmPrecedent.tm_sec - 1;

    while (true)
    {
      tmMaintenant = gestionTemps.obtenirHeureActuelle();
      gestionTemps.majDate();

      if (tmPrecedent.tm_sec != tmMaintenant.tm_sec)
      {
        // Sauvegarder les mesures toutes les 10 secondes
        if (tmMaintenant.tm_sec % 10 == 0)
        {
          gestionMesures.effectuerMesures();
          gestionMesures.sauvegarderMesures();
          std::cout << "-CSV--------------------------------CSV-\n"
                    << gestionTemps.getDateFormatee() << " | Sauvegarde des mesures\n----------------------------------------" << std::endl;
        }
        else
        { // Sinon afficher le temps avant la prochaine sauvegarde
          std::cout << gestionTemps.getDateFormatee() << " | " << std::setfill(' ') << std::setw(3) << gestionTemps.getTempsAvantProchaineSauvegarde() << "s avant la prochaine sauvegarde des mesures dans le CSV" << std::endl;
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
              std::cout << gestionTemps.getDateFormatee() << " | Erreur lors de l'écriture dans la file." << std::endl;
            }
            else
            {
              std::cout << "=LORA==============================LORA=\n"
                        << gestionTemps.getDateFormatee() << " | Envoie de la mesure dans la file IPC avec le payload : " << payload << "\n========================================" << std::endl;
            }
          }
        }
        else if (tmMaintenant.tm_min % 2 == 0 && tmMaintenant.tm_sec != 30)
        {
          if (tmMaintenant.tm_sec < 30 && tmMaintenant.tm_sec >= 0)
            std::cout << gestionTemps.getDateFormatee() << " | " << std::setfill(' ') << std::setw(3) << gestionTemps.getTempsAvantProchainEnvoiLoRa() << "s avant le prochain envoie de trame LoRa" << std::endl;
          else if (tmMaintenant.tm_sec > 30 && tmMaintenant.tm_sec < 60)
            std::cout << gestionTemps.getDateFormatee() << " | " << std::setfill(' ') << std::setw(3) << gestionTemps.getTempsAvantProchainEnvoiLoRa() << "s avant le prochain envoie de trame LoRa" << std::endl;
        }
        else if (tmMaintenant.tm_min % 2 == 1)
        {
          if (tmMaintenant.tm_sec < 30 && tmMaintenant.tm_sec >= 0)
            std::cout << gestionTemps.getDateFormatee() << " | " << std::setfill(' ') << std::setw(3) << gestionTemps.getTempsAvantProchainEnvoiLoRa() << "s avant le prochain envoie de trame LoRa" << std::endl;
          else
            std::cout << gestionTemps.getDateFormatee() << " | " << std::setfill(' ') << std::setw(3) << gestionTemps.getTempsAvantProchainEnvoiLoRa() << "s avant le prochain envoie de trame LoRa" << std::endl;
        }

        tmPrecedent = tmMaintenant;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Attendre pour ne pas surcharger avec la lecture des capteurs
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << "Exception attrapée: " << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}