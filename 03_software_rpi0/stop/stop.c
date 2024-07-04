/*
 * File:   stop.c
 * Author: philippe SIMIER  Lycée touchard Le Mans
 *
 * Created on 24 mai 2024, 14:13
 * Installer git clone https://github.com/WiringPi/WiringPi.git
 *           cd WiringPi
 *           ./build
 * ajouter l'option de compilation -l wiringPi lors de la compilation
 *
 * gcc ./stop.c -o stop -l wiringPi
 *
 */




#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define BP 25


void fallingEdgeBP(void);

int main(void) {

    if (wiringPiSetupGpio() == -1) {
        printf("Échec de l'initialisation de WiringPi\n");
            return 1;
    }

    pinMode(BP, INPUT);
    pullUpDnControl(BP, PUD_OFF);  // La résistance de tirage est sur la carte

    // Définir une interruption sur le front montant (rising edge) pour BP
    if (wiringPiISR(BP, INT_EDGE_FALLING, &fallingEdgeBP) < 0) {
        printf("Impossible de configurer l'interruption\n");
        return 1;
    }


    while (1) {

        delay(1000);
    }

    return 0;
}


void fallingEdgeBP(void) {
    printf("Détection d'un front descendant sur BP GPIO %d\n", BP);
    system("/usr/sbin/halt -p"); // chemin absolu vers la commande halt
}

