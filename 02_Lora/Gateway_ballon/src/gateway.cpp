/* 
 * File:   gateway.cpp
 * Authors: Ruben Authier, 
 *          Philippe Simier Touchard Washington
 * 
 *
 * Created on 8 février 2024, 09:51
 * Improve on 22 juin 2024 17:16
 */

#include <Arduino.h>
#include <Afficheur.h>
#include <Ttgo.h>
#include <LoRa.h>


char bufferTX[256];
int id = 0;

Ttgo ttgo;
// Fonction pour envoyer un message
void Envoyer(char* message);
// Tache pour recevoir les packets LoRa
void tacheReception(void * parameter);


void setup() {

    Serial.begin(115200);
    while (!Serial);

    ttgo.init(433775000);
    afficheur.init();
    Serial.println("Setup done");
    afficheur.afficher("Setup\nDone");
    
    xTaskCreatePinnedToCore(tacheReception, // task function
            "taskReception", // taskname
            8192,         // stack size
            NULL,         // pointer for task2 parameters
            2,            // priority
            NULL,         // handle for task
            1             // run in core 1
            );
    
}

void loop() {

    while (Serial.available() > 0) {

        // Lit le caractère suivant
        char incoming = Serial.read();
        // le renvoie en écho
        Serial.print(incoming);

        if (incoming == '\n' || incoming == '\r' || id >= 255) {

            bufferTX[id] = '\0';
            Envoyer(bufferTX);
            
            // Réinitialise l'index  pour la prochaine lecture

            id = 0;
            bufferTX[0] = '\0';

        } else {

            // si le caractère reçu est un backspace ou Suppr
            if ((incoming == 0x08 || incoming == 0x7F) && id > 0) {
                id--;
                bufferTX[id] = '\0';

            } else {
                bufferTX[id] = incoming;
                id++;
                bufferTX[id] = '\0';
            }
            afficheur.afficher(String(bufferTX));
        }
    }
}

/**
 * @brief fonction pour envoyer un packet sur LoRa
 *        attention le caratère \0 à la fin du buffer n'est pas envoyé 
 * @param message
 */
void Envoyer(char* message) {
    
    if (String(message).length() > 0) {
        Serial.print("TX : ");
        Serial.println(message);
        ttgo.send(message);
        afficheur.afficher("TX Done");
        Serial.println("TX done");
    } 

}


void tacheReception(void * parameter){
    
    String data{""};
    
    while (1) {
        data = ttgo.receive();
        Serial.println(data);
        delay(10);
    }
    
}



