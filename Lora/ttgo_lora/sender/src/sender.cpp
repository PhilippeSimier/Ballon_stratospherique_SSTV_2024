/* 
 * File:   sender.cpp
 * Author: psimier
 *
 * Created on 25 janvier 2024, 11:01
 */

#include <Arduino.h>

#include <BallonLora.h>
#include <Afficheur.h>


int counter = 0;
BallonLora bl;
char message[255];

void setup() {
    
    Serial.begin(115200);
    
    while (!Serial);
    

    bl.init();  
    afficheur.init();
    
    afficheur.afficher("Sender\nDone"); 
    
    Serial.println("LoRa Sender");
    Serial.println("Setup done");
}

void loop() {
    
    Serial.print("Sending packet: "); 
    Serial.println(counter);

    snprintf(message, sizeof (message), "Hello %03d", counter);
    
    bl.send(message);
    
    afficheur.afficher(message);
    counter++;  
    delay(10000);
}

