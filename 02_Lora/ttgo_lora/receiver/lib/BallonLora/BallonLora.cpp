/* 
 * File:   BallonLora.cpp
 * Author: psimier
 * 
 * Created on 25 janvier 2024, 11:20
 */

#include "BallonLora.h"

BallonLora::BallonLora() {
}


BallonLora::~BallonLora() {
}

void BallonLora::init(long frequency){
    
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
    LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
    LoRa.begin(frequency);
    LoRa.setSpreadingFactor(12);
    LoRa.setCodingRate4(8);
    pinMode(LED, OUTPUT);
    
}

void BallonLora::send(String message){
    
    digitalWrite(LED,1); // turn the green LED 
    
    LoRa.beginPacket();   // Démarre la séquence d'envoi d'un paquet.
    LoRa.print(message); // Ecriture des données. Chaque paquet peut contenir jusqu'à 255 octets. 
    LoRa.endPacket();     // Termine la séquence d'envoi d'un paquet.
    
    digitalWrite(LED,0); // turn the LED
}

String BallonLora::receive(){
    
    String data{""};
    int packetSize = 0;
    
    // Attend la réception d'un packet
    do {
        packetSize = LoRa.parsePacket();
    }while (!packetSize);
    
    while (LoRa.available()) {
            data = LoRa.readString();
    }
    return data;
    
}

