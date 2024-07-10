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
#include <string> 
#include "SX1278.h"

using namespace std;


int main(int argc, char** argv) {

    cout << "Programme LoRa" << endl;
       
    int8_t buffer[] = "LoRa";
    string message = "Bonjour le monde";
    
    try {

      
        loRa.begin();
      
        loRa.send(buffer, 4);
        loRa.send(message);
        
        while(1){
            sleep(1);
        }
      
       
    } catch (const std::runtime_error &e) {
        
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}

