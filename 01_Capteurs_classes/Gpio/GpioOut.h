/*
 * File:   GpioOut.h
 * Author: philippe SIMIER
 *
 * Created on 24 mai 2024, 14:13
 *
 * Installer git clone https://github.com/WiringPi/WiringPi.git
 *           cd WiringPi
 *           ./build
 * ajouter l'option de compilation -l wiringPi lors de la compilation
 * ou utiliser le Makefile
 */

#ifndef GpioOut_H
#define GpioOut_H


#include <iostream>
#include <wiringPi.h>


class GpioOut {

public:
    
    GpioOut( unsigned _gpio );
    GpioOut(const GpioOut& orig);
    virtual ~GpioOut();
    
    void setOn();
    void setOff();
    
private:
    
    unsigned gpio;

};

#endif /* LED_H */

