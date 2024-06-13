/*
 * File:   main.c
 * Authors: ale psr
 *
 * Created on 9 juin 2023, 20:18
 * from https://github.com/yandievruslan/sx1278-lora-raspberrypi
 * on rpi must install pigpio before
 * sudo apt install pigpio

   compilation : gcc *.c -lpigpio -lpthread -lm -o lora_files
   pour lister les files utiliser la commande  ipcs

 * Edit config.ini according to lora.h and RA-02 GPIO
[modem]
spiCS= 0
bw=BW125
sf=SF12
ecr=CR5
CRC=1
freq=433775000
resetGpioN=0
dio0GpioN=22
header=<xxf4kmn-9>APLT00,WIDE1-1:
 *
 *
 * linker additionnal option : -lpigpio -lrt -pthread -lm
 * run command : sudo "${OUTPUT_PATH}"
 *
 * pour activer la gestion des files dans le raspeberry
 * sudo nano /boot/config.txt
 * ajouter à la fin :
 * kernel.msgmni=1024
 * puis redémarrer
 * sudo reboot
 * usage : sudo ./rpi_lora_tx
 *
 * Console result
 * Config du fichier ini
 * spiCS = 0
 * bw = 112
 * sf = 192
 * ecr = 2
 * CRC = 1
 * freq = 433775000.000000
 * resetGpioN = 0
 * dio0GpioN = 22
 * 
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
#include "LoRa.h"
#include "readConfig.h"



typedef struct {
	long type;
	char texte[255];
}donnees;


int idFileRX;  //global pas le choix pour le handler de réception
donnees fileRX;

// Prototype des callbacks

void tx_f(txData *tx); 
void *rx_f(void *p); 


int main(int argc, char** argv) {

    printf("Programme LoRa Files \n\r");
    char txbuf[255];
    char header[255];
    LoRa_ctl modem;
    int ret;
    int idFileTX;
    int debug = 0;

    donnees fileTX;

    fileRX.type=2;
    idFileRX = msgget((key_t) 5678, 0666 | IPC_CREAT);
    if (idFileRX == -1) {
        printf("pb creation file : %s\n", strerror(errno));
        exit(errno);
    }

    idFileTX = msgget((key_t) 5679, 0666 | IPC_CREAT);
    if (idFileTX == -1) {
        printf("pb creation file : %s\n", strerror(errno));
        exit(errno);
    }

    readConfiguration(&modem, header);
    if (debug) { // Affiche les valeurs de la configuration du modem
        printf("lecture du fichier config.ini\n\r");
        printf("spiCS = %d\n\r", modem.spiCS);
        printf("bw   = %d\n\r",  modem.eth.bw);
        printf("sf   = %d\n\r",  modem.eth.sf);
        printf("ecr  = %d\n\r",  modem.eth.ecr);
        printf("CRC  = %d\n\r",  modem.eth.CRC);
        printf("freq = %f\n\r",  modem.eth.freq);
        printf("resetGpioN = %d\n\r", modem.eth.resetGpioN);
        printf("dio0GpioN  = %d\n\r", modem.eth.dio0GpioN);
    }

    modem.rx.data.userPtr = (void *)(&modem); //To handle with chip from rx callback
    modem.tx.data.userPtr = (void *)(&modem); //To handle with chip from tx callback
    modem.rx.callback = rx_f;
    modem.tx.callback = tx_f;
    modem.tx.data.buf = txbuf;
    modem.eth.preambleLen = 6;
    modem.eth.outPower = OP20;        //Output power
    modem.eth.powerOutPin = PA_BOOST; //Power Amplifire pin
    modem.eth.AGC = 1;                //Auto Gain Control
    modem.eth.OCP = 240;              // 45 to 240 mA. 0 to turn off protection
    modem.eth.implicitHeader = 0;     //Explicit header mode
    modem.eth.syncWord = 0x12;
    //For detail information  https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V5.pdf

    int retour = LoRa_begin(&modem);
    if (retour < 0){
        printf(" problème avec LoRa_begin : %d\r\n", retour);
	exit(retour);
    }
    LoRa_receive(&modem);

    while (1) {
        
        ret = msgrcv(idFileTX, (void*) &fileTX, sizeof(txbuf), 2, IPC_NOWAIT);
        if (ret != -1) {
         
            //reception dans la file d'un payload à transmettre à la station sol

            memset(txbuf, '\0', sizeof(txbuf)); // remise à zero du packet 
            strcpy(txbuf, header);              // met l'entête  
            strcat(txbuf,fileTX.texte);         // ajoute le payload
            txbuf[1] = 0xff;                    // ecrit le champ protocole        
            txbuf[2] = 0x01;

            if (debug)
                printf("Trame APRS %s\n\r len : %d\n\r", txbuf, strlen(txbuf)); 
            modem.tx.data.size = strlen(txbuf); // donne la longeur du packet à transmettre

            LoRa_stop_receive(&modem);  // stop la réception
            LoRa_send(&modem);          // puis démarre l'émission
            if (debug) {
                printf("Tsym: %f\n\r", modem.tx.data.Tsym);
                printf("Tpkt: %f\n\r", modem.tx.data.Tpkt);
                printf("payloadSymbNb: %u\n\r", modem.tx.data.payloadSymbNb);
            }
                        
            usleep((int) modem.tx.data.Tpkt * 1000); // attend la fin de l'émission
        }
        sleep(1);
    }

    printf("end\n\r");
    LoRa_end(&modem);

    return (EXIT_SUCCESS);
}

/*
    Fonction pour afficher l'heure
*/
void afficher_heure(){

    time_t current_time;
    struct tm *time_info;
    char time_string[9];  

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);

    printf(time_string);
}

/*
   callback appelé à la fin d'une émission
*/
void tx_f(txData *tx) {
    LoRa_ctl *modem = (LoRa_ctl *)(tx->userPtr);
    afficher_heure();
    printf(" tx-->done Tpkt=%.2fms\n\r", modem->tx.data.Tpkt);
    printf("         %s\n\r\n\r", tx->buf);  //Data we've sent
    LoRa_receive(modem); //on repasse en réception
}

/*
    callback appelé à la fin d'une réception
*/

void * rx_f(void *p) {
    rxData *rx = (rxData *) p;
    afficher_heure();
    printf(" rx-->done ");
    printf("CRC_error=%d ", rx->CRC);
    printf("Data_size=%d ", rx->size);
    printf("RSSI=%ddBm ", rx->RSSI);
    printf("SNR=%.2fdB\n\r", rx->SNR);

    rx->buf[rx->size] = '\0';  // Ajoute une fin de chaîne de Caratères
    strcpy(fileRX.texte,rx->buf);
    free(p);
    printf("         %s \n\r\n\r",fileRX.texte);
    msgsnd(idFileRX, (void*) &fileRX, sizeof(fileRX.texte), IPC_NOWAIT);
    sleep(1);
    return NULL;
}
