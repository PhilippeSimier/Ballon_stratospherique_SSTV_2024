/* 
 * File:   Ttgo.h
 * Author: philippe SIMIER
 *
 * Created on 22 juin 2024, 17:53
 */

#ifndef TTGO_H
#define TTGO_H

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// LoRa pins
#define LORA_MISO 19
#define LORA_CS   18
#define LORA_MOSI 27
#define LORA_SCK   5
#define LORA_RST  23
#define LORA_IRQ  26 

// Green led 
#define LED 25

class Ttgo {
    
public:
    Ttgo();

    virtual ~Ttgo();
    
    void init(long frequency = 433775000);
    void send(char *message);
    String receive();
    
private:

};

#endif /* TTGO_H */

