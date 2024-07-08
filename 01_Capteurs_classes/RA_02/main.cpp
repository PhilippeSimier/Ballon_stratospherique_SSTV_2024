/* 
 * File:   main.cpp
 * Author: philippe SIMIER
 * 
 * Programme test unitaire classe Spi
 * test les 4 méthodes 
 *
 * Created on 7 juillet 2024, 16:14
 */

#include <cstdlib>
#include <iostream>
#include "SX1278.h"

#define REG_FIFO 0x00
#define REG_FIFO_TX_BASE_ADDR 0x0E
#define REG_FIFO_RX_BASE_ADDR 0x0F
#define TX_BASE_ADDR 0x00
#define RX_BASE_ADDR 0x00
#define REG_PAYLOAD_LENGTH 0x22

using namespace std;

int main(int argc, char** argv) {

    cout << "Programme LoRa" << endl;
    unsigned char message[] = "Coucou";
    unsigned char mode;
    
    try {

        SX1278 loRa;  // channel SPI 0
        
        loRa.begin(433775000);
        
        
        loRa.send(message, 6);
        sleep(3);
        loRa.send(message, 6);
        sleep(3);
        loRa.send(message, 6);
        sleep(3);
        


       
    } catch (const std::runtime_error &e) {
        
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}

