/* Test SPI avec RA02
 *
 * philippe SIMIER
 * 06 Juillet 2024
 *
 * gcc ./main.c -o testSPI -l wiringPi
*/

#include <wiringPi.h>
#include "wiringPiSPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


unsigned char lora_reg_read_byte(int spid, unsigned char reg);
void lora_reset(unsigned char gpio_n);

int main(void) {

    int channel = 0;
    int speed   = 32000;
    int spid;


    // Initialise WiringPi
    if (wiringPiSetupGpio() == -1) {
        printf("Échec de l'initialisation de WiringPi\n");
            return 1;
    }

    lora_reset(0);

    // Initialise le canal SPI
    if ((spid  = wiringPiSPISetup(channel, speed)) < 0) {
        // Initialisation échouée
                 fprintf(stderr, "Impossible d'initialiser le canal SPI: %s\n",
                strerror(errno));
        return errno;
    }

    unsigned char value = lora_reg_read_byte(spid, 0x42); // lecture du registre RegVersion
    printf("RegVersion : 0x%x\n", value);


    return 0;
}

void lora_reset(unsigned char gpio_n){

        pinMode (gpio_n, OUTPUT);
        digitalWrite (gpio_n, LOW);
        usleep(100);
        digitalWrite (gpio_n, HIGH);
        usleep(5000);

}


unsigned char lora_reg_read_byte(int spid, unsigned char reg){

    int ret;
    char data[2];

    data[0] = reg;
    data[1] = 0x00;

    ret = wiringPiSPIDataRW( spid, data, 2);
    if (ret == -1)
        return -1;

    return data[1];
}

