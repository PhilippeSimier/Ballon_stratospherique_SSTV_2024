/* 
 * File:   Led.cpp
 * Author: philippe SIMIER Lycée Touchard Washington
 * 
 * Created on 2 août 2023, 08:57
 */

#include "GpioOut.h"

GpioOut::GpioOut(const unsigned _gpio):
gpio(_gpio)
{
    
    if (wiringPiSetupGpio()  != 0) {
        throw std::runtime_error("Exception in constructor Led wiringPiSetup");
    }

    pinMode (gpio, OUTPUT);
}

GpioOut::GpioOut(const GpioOut& orig) {
}

GpioOut::~GpioOut() {

}

void GpioOut::setOn() {
    digitalWrite (gpio, HIGH) ;
}

void GpioOut::setOff() {
    digitalWrite (gpio, LOW) ;
}

