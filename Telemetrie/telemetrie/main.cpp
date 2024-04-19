/*
 * File:   main.cpp
 * Author: dbrochard
 *
 * Created on 27 mars 2024, 15:01
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mutex>
#include "GestionMesures.h"

const int MAX_MESSAGE_SIZE = 100; // taille maximum du message
const int FILE_KEY = 5679; // clef de la file de messages
const int MSG_FLAG = 0666 | IPC_CREAT; // flag de creation de la file de messages
const int SLEEP_DURATION_MEASUREMENTS = 10; // seconds
const int SLEEP_DURATION_LORA = 12; // seconds (2 minutes)

struct Message {
    long mtype;
    char mtext[MAX_MESSAGE_SIZE];
};

GestionMesures gestionMesures;
std::mutex globalMutex;

void envoyerMessageALaFile(int msgid, const std::string &payload) {
    Message message;
    message.mtype = 2;

    payload.copy(message.mtext, sizeof (message.mtext) - 1);
    message.mtext[sizeof (message.mtext) - 1] = '\0';

    // Utilisation de std::lock_guard pour verrouiller automatiquement
    std::lock_guard<std::mutex> lock(globalMutex);
    if (msgsnd(msgid, &message, sizeof (message.mtext), 0) == -1) {
        throw std::runtime_error("Erreur lors de l'envoi du message.");
    }

    std::cout << "Message envoyé : " << payload << std::endl;
}

int main(int argc, char **argv) {
    int msgid;

    try {

        if ((msgid = msgget((key_t) FILE_KEY, MSG_FLAG)) == -1) {
            throw std::runtime_error("Erreur lors de la création de la file de messages.");
        }

        while (true) {

            // Obtenir l'heure actuelle
            auto maintenant = chrono::system_clock::now();
            auto tempsActuel = chrono::system_clock::to_time_t(maintenant);
            auto tmMaintenant = *localtime(&tempsActuel);

            std::cout << "Seconde : " << tmMaintenant.tm_sec << std::endl;

            // Sauvegarder
            if (tmMaintenant.tm_sec % 10 == 0) {
                gestionMesures.majDate();
                gestionMesures.effectuerMesures();
                gestionMesures.sauvegarderMesures();
            }

            // Envoyer
            if (tmMaintenant.tm_min % 2 == 0 && tmMaintenant.tm_sec == 30) {
                gestionMesures.majDate();
                gestionMesures.effectuerMesures();
                if (gestionMesures.verifierMesures()) {
                    std::string payload = gestionMesures.formaterMesuresPourLora();
                    // Envoi du message dans une fonction séparée

                    envoyerMessageALaFile(msgid, payload);
                    std::cout << "envoyerMessageALaFile(msgid, payload); : " << payload << std::endl;

                }
            }
            this_thread::sleep_for(chrono::milliseconds(1000));
        }

    } catch (const std::exception &e) {
        std::cerr << "Exception attrapée: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}