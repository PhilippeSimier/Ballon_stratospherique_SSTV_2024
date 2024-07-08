/* 
 * File:   main.cpp
 * Author: philippe SIMIER
 * 
 * Programme test unitaire classe SX1278
 * test les 4 méthodes 
 *
 * Created on 7 juillet 2024, 16:14
 */

#include <cstdlib>
#include <iostream>
#include "SX1278.h"



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

