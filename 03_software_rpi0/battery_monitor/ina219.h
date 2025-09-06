/*!
    \file     ina219.h
    \author   Philippe SIMIER (Touchard Wahington le Mans)
    \license  BSD (see license.txt)
    \brief    Classe pour le composant I2C ina219
    \detail
    Pour comprendre la calibration du INA219
    la conversion se fait sur 12 bits (2 exp 12 = 4096)
    L'ADC peut mesurer des tensions comprises entre ± 40mV (± pour la mesure du courant bidirectionnel).
    le PGA est un diviseur de tension programmable par 1, 2, 4, or 8
    Cela donne à l'INA219 une plage effective de ± 40 mV, ± 80 mV, ± 160mV, ou ± 320mV respectivement.
    http://cdwilson.us/articles/understanding-the-INA219
    \version  v1.0 - First release
*/

#ifndef INA219_H_INCLUDED
#define INA219_H_INCLUDED

#include <iostream>
#include <iomanip>
#include "i2c.h"
#include <stdint.h>       // pour les types uint8_t etc
#include <math.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <limits>       // std::numeric_limits

#define ADRESSE_I2C_DEFAUT              0x40  // adresse I2C par défaut pour ina219
//Adresse des Registres internes du composant
#define REG_CONFIG      0x00
#define REG_SHUNT       0x01
#define REG_BUS         0x02
#define REG_POWER       0x03
#define REG_CURRENT     0x04
#define REG_CALIBRATION 0x05

#define BVOLTAGERANGE_MASK        (0x2000)  // Bus Voltage Range Mask
#define BVOLTAGERANGE_16V         (0x0000)  // plage 0-16V
#define BVOLTAGERANGE_32V         (0x2000)  // plage 0-32V

#define GAIN_MASK                 (0x1800)  // Gain Masque
#define GAIN_1                    (0x0000)  // Gain 1, plage 40mV
#define GAIN_2	                  (0x0800)  // Gain 2, plage 80mV
#define GAIN_4	                  (0x1000)  // Gain 4, plage 160mV
#define GAIN_8	                  (0x1800)  // Gain 8, plage 320mV

#define BADCRES_MASK              (0x0780)  // Bus ADC Resolution Mask
#define BADCRES_9BIT              (0x0080)  // 9-bit bus res = 0..511
#define BADCRES_10BIT             (0x0100)  // 10-bit bus res = 0..1023
#define BADCRES_11BIT             (0x0200)  // 11-bit bus res = 0..2047
#define BADCRES_12BIT             (0x0400)  // 12-bit bus res = 0..4095

#define SADCRES_MASK              (0x0078)  // Shunt ADC Resolution and Averaging Mask
#define SADCRES_9BIT_1S           (0x0000)  // 1 x 9-bit shunt sample 84us
#define SADCRES_10BIT_1S          (0x0008)  // 1 x 10-bit shunt sample 148 us
#define SADCRES_11BIT_1S          (0x0010)  // 1 x 11-bit shunt sample 276 us
#define SADCRES_12BIT_1S          (0x0018)  // 1 x 12-bit shunt sample 532 us
#define SADCRES_12BIT_2S          (0x0048)	// 2 x 12-bit shunt samples averaged together 1060us
#define SADCRES_12BIT_4S          (0x0050)  // 4 x 12-bit shunt samples averaged together 2130us
#define SADCRES_12BIT_8S   	      (0x0058)  // 8 x 12-bit shunt samples averaged together 4260us
#define SADCRES_12BIT_16S         (0x0060)  // 16 x 12-bit shunt samples averaged together 8510us
#define SADCRES_12BIT_32S         (0x0068)  // 32 x 12-bit shunt samples averaged together 17ms
#define SADCRES_12BIT_64S	      (0x0070)  // 64 x 12-bit shunt samples averaged together 34 ms
#define SADCRES_12BIT_128S	      (0x0078)  // 128 x 12-bit shunt samples averaged together 69 ms

#define MODE_MASK                 (0x0007)  // Operating Mode Mask
#define MODE_POWERDOWN            (0x0000)
#define MODE_SVOLT_TRIGGERED      (0x0001)
#define MODE_BVOLT_TRIGGERED      (0x0002)
#define MODE_SANDBVOLT_TRIGGERED  (0x0003)
#define MODE_ADCOFF               (0x0004)
#define MODE_SVOLT_CONTINUOUS     (0x0005)
#define MODE_BVOLT_CONTINUOUS     (0x0006)
#define MODE_SANDBVOLT_CONTINUOUS (0x0007)

class ina219
{

public:

    ina219(int i2cAddress=ADRESSE_I2C_DEFAUT, float _quantum=3.991);

    ~ina219();

    float obtenirTension_V();
    float obtenirCourant_mA();
    float obtenirPuissance_W();

    void  fixerCalibration_16V();

private:
    i2c *deviceI2C;            // file descriptor

    float quantum;		      // la valeur du Quantum en mV
    float shunt;		      // la valeur du shunt en ohm

    // Fonction pour réaliser une adaptation d'échelle de valeur
    float map(float x, float in_min, float in_max, float out_min, float out_max);
};

#endif // INA219_H_INCLUDED
