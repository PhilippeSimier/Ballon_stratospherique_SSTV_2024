/* 
 * File:   BallonLora.h
 * Author: psimier
 *
 * Created on 25 janvier 2024, 11:20
 */

#ifndef BALLONLORA_H
#define BALLONLORA_H

// LoRa pins
#define LORA_MISO 19
#define LORA_CS   18
#define LORA_MOSI 27
#define LORA_SCK   5
#define LORA_RST  23
#define LORA_IRQ  26 

// Green led 
#define LED 25 

#include <Arduino.h>

#include <SPI.h>
#include <LoRa.h>

class BallonLora {

public:
    
    BallonLora();
    
    virtual ~BallonLora();
    void init(long frequency = 433775000);
    void send(String message);
    String receive();

private:

};

#endif /* BALLONLORA_H */

