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
#include "Spi.h"

#define REG_FIFO 0x00
#define REG_FIFO_TX_BASE_ADDR 0x0E
#define REG_FIFO_RX_BASE_ADDR 0x0F
#define TX_BASE_ADDR 0x00
#define RX_BASE_ADDR 0x00
#define REG_PAYLOAD_LENGTH 0x22

using namespace std;

int main(int argc, char** argv) {

    cout << "Programme LoRa" << endl;

    try {

        Spi spi;

        auto value = spi.read_reg(0x42);
        cout << "RegVersion : " << hex << (int)value << endl;
        
        spi.write_reg(0x40, 1<<6);
        value = spi.read_reg(0x40);
        cout << "Reg 0x40 : " << hex << (int)value << endl;
        
        unsigned char chaine[] = "Bonjour\0";
        spi.write_reg(REG_FIFO_TX_BASE_ADDR, TX_BASE_ADDR);
        spi.write_reg(REG_FIFO_RX_BASE_ADDR, RX_BASE_ADDR);
        spi.write_reg(REG_PAYLOAD_LENGTH, 8);
        
        spi.write_fifo(REG_FIFO, chaine, 8);
        unsigned char buffer[255] = {};
        spi.read_fifo(REG_FIFO, buffer, 8);
        
        cout << buffer << endl;
        
    } catch (const std::runtime_error &e) {
        
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}

