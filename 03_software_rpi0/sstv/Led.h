/* 
 * File:   Led.h
 * Author: philippe SIMIER
 *
 * Created on 2 août 2023, 08:57
 */

#ifndef LED_H
#define LED_H


#include <iostream>
#include <pigpio.h>


class Led {
public:
    
    Led( unsigned _gpio );
    Led(const Led& orig);
    virtual ~Led();
    
    void setOn();
    void setOff();
    
private:
    
    unsigned gpio;

};

#endif /* LED_H */

