/* 
 * File:   main.cpp
 * Author: philippe SIMIER Lycée Touchard Washington
 * 
 * Programme transmission radio LoRa avec la classe SX1278
 * test les 2 méthodes continuous_receive() & send() 
 * test l'operateur << 
 * test la configuration à partir d'un fichier .ini
 *
 * Created on 7 juillet 2024, 16:14
 */

#include <cstdlib>
#include <iostream>
#include <string> 
#include "SX1278.h"
#include "GestionFile.h"
#include "SimpleIni.h"

#define CONFIGURATION "/home/ballon/configuration.ini"

using namespace std;

void callback_Rx(char* payload, int rssi, float snr); // user callback function for when a packet is received. 
void callback_Tx(void); // user callback function for when a packet is transmited.

GestionFile fileRX(5678);  // Objets pour la gestion de la file des messages reçus key 5678
GestionFile fileTX(5679);  // file pour les messages emis key 5679

int main(int argc, char** argv) {

    cout << "Start lora_files" << endl;
    SimpleIni ini;  
    Payload payload;

    try {

        ini.Load(CONFIGURATION);
        loRa.onRxDone(callback_Rx); // Register a user callback function 
        loRa.onTxDone(callback_Tx); // Register a user callback function
        
        
        loRa.begin( ini.GetValue<double>("modem", "freq", 433775000));      // settings the radio
        loRa.set_bandwidth( loRa.bwFromDouble(ini.GetValue<double>("modem", "bw", 125)));     // setting the BandWidth
        loRa.set_ecr( loRa.ecrFromString( ini.GetValue("modem", "ecr", "CR5")));
        loRa.set_sf( loRa.sfFromString( ini.GetValue("modem", "sf", "SF12")));
        
        loRa.continuous_receive();  // Puts the radio in continuous receive mode.

        sleep(1);

        string indicatif = ini.GetValue("aprs", "indicatif", "F4ABC");
        string path = ini.GetValue("aprs", "path", "WIDE1-1");
        string to = ini.GetValue("aprs", "to", "APLPS0");
        loRa << beginPacket << "<\xff\x01" << indicatif << ">" << to << "," << path << ":";
        loRa << ":f4kmn    :Ballon Touchard QRV" << endPacket;

        while (1) {
            payload = fileTX.read(2);
            loRa << beginPacket << "<\xff\x01" << indicatif << ">" << to << "," << path << ":";
            loRa << payload.text << endPacket;
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
    fileRX.write(payload, rssi, snr);
    

}

void callback_Tx(void) {

}


