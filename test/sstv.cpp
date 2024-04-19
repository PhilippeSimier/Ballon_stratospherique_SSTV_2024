/*
*  Programme test pour lancer un processus ./lora_file
*  à partir du processus sstv
*  Commande pour voir les processus :  ps -af
*  Commande pour envoyer un signal SIGUSR1 au processus lora_file : 
*  kill -SIGUSR1 6791
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    FILE *fp;
    char command[100];
    int pid;

    // Commande à exécuter
    sprintf(command, "./lora_file & echo $!");

    // Exécute la commande et récupère le PID
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Erreur lors de l'exécution de la commande\n");
        exit(1);
    }
    fscanf(fp, "%d", &pid);

    // Ferme le descripteur de fichier
    pclose(fp);

    // Affiche le PID
    printf("PID du processus : %d\n", pid);

    // Faites d'autres opérations si nécessaire...
    sleep(20);

    // Envoi du signal SIGUSR1 au processus cible
    if(kill(pid, SIGUSR1) == 0) {
        printf("Signal SIGUSR1 envoyé au processus avec PID %d.\n", pid);
    } else {
        perror("Erreur lors de l'envoi du signal SIGUSR1");
        return 1;
    }


    return 0;
}

