/*
 * File:   main.c
 * Author: philippe SIMIER Touchard Washington
 *
 * Compilation: gcc LM75.c -lwiringPi
 *
 * Exécution   ./a.out 
 * 
 * Created on 20 juillet 2023, 11:08
 */

#include <wiringPiI2C.h>
#include <stdio.h>

int main(void) {

    int handle = wiringPiI2CSetup(0x48);
    int reg0 = wiringPiI2CReadReg16(handle, 0x00);
    int temp = reg0 & 0x00ff;
    int half = 0;

    if (reg0 & 0xff00)
        half = 5;
    else
        half = 0;

    printf("Temp:%d.%d\n", temp, half);
    return 0;
}
