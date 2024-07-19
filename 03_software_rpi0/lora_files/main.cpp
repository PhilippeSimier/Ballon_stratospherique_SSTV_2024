/* 
 * File:   main.cpp
 * Author: philippe SIMIER Lycée Touchard Washington
 * 
 * Programme test unitaire classe SX1278
 * test les 2 méthodes continuous_receive() & send() 
 * test l'operateur << 
 *
 * Created on 7 juillet 2024, 16:14
 */

#include <cstdlib>
#include <iostream>
#include <string> 
#include "SX1278.h"
#include "GestionFile.h"

using namespace std;

void callback_Rx(char* payload, int rssi, float snr); // user callback function for when a packet is received. 
void callback_Tx(void); // user callback function for when a packet is transmited.

GestionFile fileRX; // Objets pour la gestion de la file des messages reçus
GestionFile fileTX;

int main(int argc, char** argv) {

    cout << "Programme LoRa" << endl;
    MessageTX message;

    try {


        fileRX.obtenirFileIPC(5678); // Obtenir la file pour la réception key 5678
        fileTX.obtenirFileIPC(5679); // Obtenir la file pour l'émission   key 5679

        loRa.onRxDone(callback_Rx); // Register a user callback function 
        loRa.onTxDone(callback_Tx); // Register a user callback function
        loRa.begin(433775000); // settings the radio     
        loRa.continuous_receive(); // Puts the radio in continuous receive mode.

        sleep(1);

        loRa.send("Welcome LoRa");
 
        while (1) {
            message = fileTX.readFileTX(2);
            loRa << beginPacket << "<\xff\x01" << "f4JRE-9>APLT00,WIDE1-1:";
            loRa << message.text << endPacket;
            sleep(1);
        }

    } catch (const std::runtime_error &e) {
        cout << "Exception caught: " << e.what() << endl;
    }
    return 0;
}

/**
 * @brief Callback utilisateur appelé après la reception compléte 
 *        d'un packet
 * @param buffer une chaine de caratères char*
 * @param rssi  le niveau de reception dBm
 * @param snr   le rapport signal / bruit
 */
void callback_Rx(char* payload, int rssi, float snr) {
    cout << "Rx done : " << payload;
    cout << " RSSI : " << rssi << "dBm";
    cout << " SNR  : " << snr << "dB" << endl;
    fileRX.writeFileRX(payload, rssi, snr);
    

}

void callback_Tx(void) {
    cout << "Tx done : " << endl;
}


