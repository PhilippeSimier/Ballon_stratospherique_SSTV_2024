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
#include "bme280.h"
#include "GpioOut.h"

using namespace std;

// Fonction pour lancer le processus lora_files
int run_lora_files() {

    FILE *fp;
    char command[100];
    int pid;

    // Commande à exécuter pour lancer lora_files
    sprintf(command, "/home/ballon/lora_files/lora_files >> log.txt & echo $!");
    // Exécute la commande et récupère le PID
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'exécution de la commande\n");
        exit(1);
    }
    fscanf(fp, "%d", &pid);

    // Ferme le descripteur de fichier
    pclose(fp);
    return pid;

}

// Fonction pour stoper le processus lora_files
int stop_lora_files(int pid) {

    int retour = 0;
    // Envoi du signal SIGTERM au processus cible
    if(kill(pid, SIGTERM ) == 0) {
        printf("Signal SIGTERM envoyé au processus avec PID %d.\n", pid);
    } else {
        perror("Erreur lors de l'envoi du signal SIGUSR1");
        retour = 1;
    }
    return retour;
}


/**
 *    Fonction exécutée par le deuxième thread
 */
void threadFunction1() {

    int pid;
    pid = run_lora_files();
    GpioOut ptt(6);  // Commande Push To Talk

    while (true) {

        // Obtenir l'heure actuelle
        auto maintenant = chrono::system_clock::now();
        auto tempsActuel = chrono::system_clock::to_time_t(maintenant);
        auto tmMaintenant = *localtime(&tempsActuel);

        // Vérifier si c'est un multiple de 5 minutes - 6s
        if (tmMaintenant.tm_sec == 54 && tmMaintenant.tm_min % 5 == 4) {

            // stopper lora_files
	    stop_lora_files(pid);

            // Appeler la methode envoyer photo en SSTV
            ptt.setOn(); // active l'ampli
            Camera::envoyerPhoto(29000000);
            this_thread::sleep_for(chrono::seconds(1));
            ptt.setOff(); // désactive l'ampli

	    // redémarrer lora_files
	    pid = run_lora_files();

        }
    }
}


int main(int argc, char** argv) {

    BME280 capteur(0x77);
    Camera camera;

    thread t1(threadFunction1);
    while (true) {

        // Obtenir l'heure actuelle
        auto maintenant = chrono::system_clock::now();
        auto tempsActuel = chrono::system_clock::to_time_t(maintenant);
        auto tmMaintenant = *localtime(&tempsActuel);

        if (tmMaintenant.tm_sec == 30) {

            stringstream annotation;
            annotation << fixed << setprecision(1) << capteur.obtenirPression() << " hPa";
            cout << "Pression : " << annotation.str() << endl;
            camera.enregistrerPhoto(annotation.str());

            this_thread::sleep_for(chrono::seconds(1));

        }
    }
    // Attente de la fin du thread t1
    t1.join();

    return 0;

}

