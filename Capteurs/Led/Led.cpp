/* 
 * File:   Led.cpp
 * Author: philippe SIMIER
 * 
 * Created on 2 août 2023, 08:57
 */

#include "Led.h"

Led::Led(const unsigned _gpio):
gpio(_gpio)
{
    
    if (gpioInitialise() < 0) {
        throw std::runtime_error("Exception in constructor class Led");
    }

    if (gpioSetMode(gpio, PI_OUTPUT) != 0){
        throw std::runtime_error("Exception in constructor Led gpioSetMode");
    }
}

Led::Led(const Led& orig) {
}

Led::~Led() {
    gpioTerminate();
}

void Led::setOn() {
    gpioWrite(gpio, 1);
}

void Led::setOff() {
    gpioWrite(gpio, 0);
}

