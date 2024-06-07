/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: ale
 * ne pas oublier
 * 
 * sudo apt install libncurses-dev
 * linker additionnal option : -lncurses
  */

#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <ncurses.h> 

//callback

typedef struct {
	long type;
	char texte[255];
}donnees;



int main(int argc, char** argv) {

    int idFileRX,idFileTX;
    donnees fileTX,fileRX;
    int ret;
    char key=' ';
    
    initscr();  // Initialisation de ncurses
    cbreak();   // Lecture des caractères un par un, sans attente
    echo();   // activation de l'écho des caractères saisis
    nodelay(stdscr, TRUE);  // Mode non bloquant pour getch() 
    
    
    fileTX.type=2;   
    idFileRX = msgget((key_t) 5678, 0666 | IPC_CREAT);
    if (idFileRX == -1) {
        printf("pb creation file : %s\n", strerror(errno));
        exit(0);
    }
    
    idFileTX = msgget((key_t) 5679, 0666 | IPC_CREAT);
    if (idFileTX == -1) {
        printf("pb creation file : %s\n", strerror(errno));
        exit(0);
    }
    
    while (key!='q') {
        //file de réception en provenance de la liaison montante
        memset(fileRX.texte, '\0', 255);
        ret = msgrcv(idFileRX, (void*) &fileRX, sizeof(fileRX.texte), 2, IPC_NOWAIT);
        if (ret != -1) {
            printf("Trame APRS recue %s %d\n\r", fileRX.texte, strlen(fileRX.texte));
        }
        //file de transmission vers la station sol si touche t appuyée
        key = getch();
        if (key != ERR) {  // Une touche a été pressée
            // Quitter si la touche 'q' est pressée
            if (key == 't') {
                printf("envoi vers la file\n\r");
                memset(fileTX.texte, '\0', sizeof(fileTX.texte));
                strcpy(fileTX.texte,"!4753.42N/00016.61Eb/A=005000");  //payload
                msgsnd(idFileTX, (void*) &fileTX, sizeof(fileTX.texte), IPC_NOWAIT);                
            }
        }
        sleep(1);
        
    }
    endwin();    
    printf("exit\n\r"); 
    return (EXIT_SUCCESS);
}
