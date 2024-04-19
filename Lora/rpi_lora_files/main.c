/*
 * File:   main.c
 * Authors: ale psr
 *
 * Created on 9 juin 2023, 20:18
 * from https://github.com/yandievruslan/sx1278-lora-raspberrypi
 * on rpi must install pigpio before
 * sudo apt install pigpio
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
 * réception en provenance de la station sol
 * rx --------------------------------> done réception en provenance de la station sol
CRC error: 0
Data size: 70
string: <▒f4goh-9>APLT00,WIDE1-1:!4753.41N/00016.61Eb/A=000271Bat:4.06V/-99mA▒~▒e▒~
RSSI: -46
SNR: 12.500000
buffer <▒f4goh-9>APLT00,WIDE1-1:!4753.41N/00016.61Eb/A=000271Bat:4.06V/-99mA▒~▒e▒~
Trame APRS <▒f4kmn-9>APLT00,WIDE1-1:!4753.42N/00016.61Eb/A=001000 55
Tsym: 32.768000
Tpkt: 2400.256000
payloadSymbNb: 63
sleep 2 seconds to transmit complete
 * envoi vers la station sol
tx --------------------------------> done;
sent string: "<▒f4kmn-9>APLT00,WIDE1-1:!4753.42N/00016.61Eb/A=001000"
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

// Callback

void tx_f(txData *tx) {
    LoRa_ctl *modem = (LoRa_ctl *)(tx->userPtr);
    printf("tx --------------------------------> done;\n\r");
    printf("sent string: \"%s\"\n\n", tx->buf);//Data we've sent
    LoRa_receive(modem);
}

void * rx_f(void *p) {
    rxData *rx = (rxData *) p;
    printf("rx --------------------------------> done \n\r");
    printf("CRC error: %d\n\r", rx->CRC);
    printf("Data size: %d\n\r", rx->size);
    rx->buf[rx->size] = '\0';  // Ajoute une fin de chaîne de Caratères
    printf("RSSI: %d dBm\n\r", rx->RSSI);
    printf("SNR: %f  dB\n\r", rx->SNR);
    strcpy(fileRX.texte,rx->buf);
    free(p);
    printf("buffer %s \n\r",fileRX.texte);
    msgsnd(idFileRX, (void*) &fileRX, sizeof(fileRX.texte), IPC_NOWAIT);
    sleep(1);
    return NULL;
}

int main(int argc, char** argv) {

    printf("Programme LoRa \n\r");
    char txbuf[255];
    char header[255];
    char *payload="!4753.42N/00016.61Eb/A=000261\0";
    LoRa_ctl modem;
    int ret;
    int idFileTX;
    donnees fileTX;

    fileRX.type=2;
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

    readConfiguration(&modem, header);

    printf("Configuration à partir de config.ini\n\r");
    // Afficher les valeurs de la structure
    printf("spiCS = %d\n\r", modem.spiCS);
    printf("bw   = %d\n\r",    modem.eth.bw);
    printf("sf   = %d\n\r",    modem.eth.sf);
    printf("ecr  = %d\n\r",   modem.eth.ecr);
    printf("CRC  = %d\n\r",   modem.eth.CRC);
    printf("freq = %f\n\r",  modem.eth.freq);
    printf("resetGpioN = %d\n\r", modem.eth.resetGpioN);
    printf("dio0GpioN  = %d\n\r", modem.eth.dio0GpioN);

    modem.rx.data.userPtr = (void *)(&modem);//To handle with chip from rx callback
    modem.tx.data.userPtr = (void *)(&modem);//To handle with chip from tx callback
    modem.rx.callback = rx_f;
    modem.tx.callback = tx_f;
    modem.tx.data.buf = txbuf;
    //modem.tx.data.size = strlen(txbuf); //Payload len
    modem.eth.preambleLen = 6;
    modem.eth.outPower = OP20; //Output power
    modem.eth.powerOutPin = PA_BOOST; //Power Amplifire pin
    modem.eth.AGC = 1; //Auto Gain Control
    modem.eth.OCP = 240; // 45 to 240 mA. 0 to turn off protection
    modem.eth.implicitHeader = 0; //Explicit header mode
    modem.eth.syncWord = 0x12;
    //For detail information about SF, Error Coding Rate, Explicit header, Bandwidth, AGC, Over current protection and other features refer to sx127x datasheet https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V5.pdf

    LoRa_begin(&modem);
    LoRa_receive(&modem);
    
    //gpioTerminate();
    
    while (1) {
        //reception dans la file d'une trame à transmettre à la station sol
        ret = msgrcv(idFileTX, (void*) &fileTX, sizeof(txbuf), 2, IPC_NOWAIT);
        if (ret != -1) {
            printf("Reception d'une trame à émettre en LoRa\r\n");
	    printf("%s\r\n", fileTX.texte);	
            LoRa_stop_receive(&modem);  //stop la réception

            memset(txbuf, '\0', sizeof(txbuf)); //header + payload de la file
            strcpy(txbuf, header);
            strcat(txbuf,fileTX.texte);
            txbuf[1] = 0xff;
            txbuf[2] = 0x01;
            printf("Trame APRS %s\n\r len : %d\n\r", txbuf, strlen(txbuf)); 
            modem.tx.data.size = strlen(txbuf); //Payload len
            LoRa_send(&modem);              //c'est parti, confirmation dans le handler tx_f
            printf("Tsym: %f\n\r", modem.tx.data.Tsym);
            printf("Tpkt: %f\n\r", modem.tx.data.Tpkt);
            printf("payloadSymbNb: %u\n\r", modem.tx.data.payloadSymbNb);

            printf("sleep %d seconds to transmit complete\n\r", (int) modem.tx.data.Tpkt / 1000);
            sleep(((int) modem.tx.data.Tpkt / 1000) + 1);
            LoRa_receive(&modem);  //on repasse en réception
        }
        sleep(1);
    }

    printf("end\n\r");
    LoRa_end(&modem);

    return (EXIT_SUCCESS);
}
