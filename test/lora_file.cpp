#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Définition d'un flag pour indiquer si SIGUSR1 a été reçu
volatile sig_atomic_t sigusr1_received = 0;

// Gestionnaire de signal pour SIGUSR1
void sigusr1_handler(int signum) {
    sigusr1_received = 1;
}

int main() {
    // Enregistrement du gestionnaire de signal
    signal(SIGUSR1, sigusr1_handler);

    printf("Début du processus Lora_file\n");

    // Boucle infinie
    while(1) {
        // Vérification si SIGUSR1 a été reçu
        if(sigusr1_received) {
            printf("Signal SIGUSR1 reçu. \n");
            break; // Sortie de la boucle
        }
        // Autre traitement dans la boucle

        sleep(1); // Pour éviter le débordement du processeur
    }
    printf("Fin de processus lora_file\n");
    return 0;
}

