/* 
 * File:   Ttgo.cpp
 * Author: philippe
 * 
 * Created on 22 juin 2024, 17:53
 */

#include "Ttgo.h"

Ttgo::Ttgo() {
}

Ttgo::~Ttgo() {
}

void Ttgo::init(long frequency) {

    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
    LoRa.begin(frequency);
    LoRa.setSpreadingFactor(12);
    LoRa.setCodingRate4(8);
    pinMode(LED, OUTPUT);

}

void Ttgo::send(char *message) {

    digitalWrite(LED, 1); // turn the green LED 

    LoRa.beginPacket(); // Démarre la séquence d'envoi d'un paquet.
    LoRa.print(message); // Ecriture des données. Chaque paquet peut contenir jusqu'à 255 octets. 
    LoRa.endPacket(); // Termine la séquence d'envoi d'un paquet.

    digitalWrite(LED, 0); // turn the LED
}

String Ttgo::receive() {

    String data{""};
    int packetSize = 0;

    // Attend la réception d'un packet
    do {
        packetSize = LoRa.parsePacket();
        delay(10);
    } while (!packetSize);

    while (LoRa.available()) {      
        data = LoRa.readString();
        delay(10);
    }
    return data;

}

