/* 
 * File:   main.cpp
 * Author: psimier
 *
 * Created on 26 janvier 2024, 13:13
 */

#include <BallonLora.h>
#include <Afficheur.h>

String data{""};
BallonLora bl;

void setup() {
    
    Serial.begin(115200);
    
    while (!Serial);
    

    bl.init();  
    afficheur.init();
    
    afficheur.afficher("Receiver\nDone"); 
    
    Serial.println("LoRa Receiver");
    Serial.println("Setup done");
}

void loop() {
    
    
    data = bl.receive();
    afficheur.afficher(data);
    
}



