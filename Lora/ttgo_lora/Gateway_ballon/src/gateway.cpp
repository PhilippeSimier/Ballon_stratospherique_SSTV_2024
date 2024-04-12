/* 
 * File:   gateway.cpp
 * Author: rauthier
 *
 * Created on 8 février 2024, 09:51
 */

#include <Arduino.h>
#include <Afficheur.h>
#include <Ttgo.h>
#include <LoRa.h>


char receivedString[50];
int id = 0;

void setup() {

    Serial.begin(9600);
    while (!Serial);


    pinMode(LED, OUTPUT);

    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);

    // Initialize LoRa with the specified frequency 433,775 Mhz.
    if (!LoRa.begin(433775000)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    LoRa.setSpreadingFactor(12);
    LoRa.setCodingRate4(8);

    afficheur.init();
    Serial.println("Setup done");
    afficheur.afficher("Sender\nDone");


}

void loop() {



    while (Serial.available() > 0) {

        // Lit le caractère suivant
        char incomingCharacter = Serial.read();

        // Vérifie si le caractère reçu est un retour à la ligne ou une nouvelle ligne
        if (incomingCharacter == '\n' || incomingCharacter == '\r') {
            // Si c'est le cas, ajoute un caractère de fin de chaîne
            receivedString[id] = '\0';

            // Affiche la chaîne reçue sur le port série
            Serial.print("Chaîne reçue : ");
            Serial.println(receivedString);

            Serial.print("index : ");
            Serial.println(id);

            afficheur.afficher(String(receivedString));


            // send message
            digitalWrite(LED, 1); // turn the LED
            LoRa.beginPacket(); // Démarre la séquence d'envoi d'un paquet.
            LoRa.print(String(receivedString)); // Ecriture des données. Chaque paquet peut contenir jusqu'à 255 octets. 
            LoRa.endPacket(); // Termine la séquence d'envoi d'un paquet.
            digitalWrite(LED, 0); // turn the LED

            Serial.println("OK");

            // Réinitialise l'index pour la prochaine lecture
            id = 0;
        } else {
            // Si ce n'est pas un caractère de fin de ligne, ajoute le caractère à la chaîne
            receivedString[id] = incomingCharacter;
            // Passe au caractère suivant dans la chaîne
            id++;

            // Vérifie si la longueur maximale de la chaîne est atteinte
            if (id >= 49) {
                // Ajoute un caractère de fin de chaîne
                receivedString[id] = '\0';
                // Affiche la chaîne reçue sur le port série
                Serial.println("Chaîne reçue : " + String(receivedString));
                // Réinitialise l'index pour la prochaine lecture
                id = 0;
            }

        }


    }

}

