/*
 * File:   stop.c
 * Author: philippe SIMIER
 *
 * Created on 24 mai 2024, 14:13
 * Installer git clone https://github.com/WiringPi/WiringPi.git
 *           cd WiringPi
 *           ./build
 * ajouter l'option de compilation -l wiringPi lors de la compilation
 *
 * gcc ./stop.c -o stop -l wiringPi
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h> // Include WiringPi library!

int main(void)
{

  wiringPiSetupGpio();

  // pin mode ..(INPUT, OUTPUT, PWM_OUTPUT, GPIO_CLOCK)
  // set pin 25 to input (BP)
  pinMode(25, INPUT);

  // pull up/down mode (PUD_OFF, PUD_UP, PUD_DOWN) => down
  pullUpDnControl(25, PUD_OFF);

  while(1) {
      // get state of pin 25 (BP)
      int value = digitalRead(25);

      if (value == LOW)
      {
          printf("bouton pousoir actionné\r\n");
          system("/usr/sbin/halt -p"); // chemin absolu vers la commande halt
      }

      sleep(1);
  }
}
