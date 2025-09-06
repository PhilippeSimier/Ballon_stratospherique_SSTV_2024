/*!
    \file     ina219.cpp
    \author   Philippe SIMIER (F4JRE)
    \license  BSD (see license.txt)
    \brief    Classe pour le composant i2c  INA219
    \detail   Pour comprendre la calibration du INA219
    	la conversion se fait sur 12 bits (2 exp 12 = 4096)
    	L'ADC peut mesurer des tensions comprises entre ± 40mV (± pour la mesure du courant bidirectionnel).
    	le PGA est un diviseur de tension programmable par 1, 2, 4, or 8
    	Cela donne à l'INA219 une plage effective de ± 40 mV, ± 80 mV, ± 160mV, ou ± 320mV respectivement.
    	http://cdwilson.us/articles/understanding-the-INA219
    \version    1.0 - First release
*/

#include "ina219.h"
#include "i2c.h"

using namespace std;


ina219::ina219(int i2cAddress, float  _quantum)
{
    deviceI2C = new i2c(i2cAddress);
    if (deviceI2C->getError()){
        throw std::runtime_error("Exception in constructeur ina219");
    }

    deviceI2C->WriteReg16(REG_CONFIG, 0x0080);   // reset

    short rc = 4096;
    rc = ((rc & 0x00FF) << 8) | ((rc & 0xFF00) >> 8);
    deviceI2C->WriteReg16(REG_CALIBRATION , rc);  // écriture du registre de calibration

    uint16_t config;
    config  = BVOLTAGERANGE_32V | GAIN_8 | BADCRES_12BIT | SADCRES_12BIT_128S | MODE_SANDBVOLT_CONTINUOUS;
    config = ((config & 0x00FF) << 8) | ((config & 0xFF00) >> 8);  // inversion msb lsb
    deviceI2C->WriteReg16(REG_CONFIG, config);

    quantum = _quantum;

}


ina219::~ina219()
{
    if (deviceI2C != NULL)
        delete deviceI2C;
}


/*!
    @brief  Set calibration at 16V
*/

void ina219::fixerCalibration_16V(){

    uint16_t config;
    config  = BVOLTAGERANGE_16V | GAIN_8 | BADCRES_12BIT | SADCRES_12BIT_128S | MODE_SANDBVOLT_CONTINUOUS;
    config = ((config & 0x00FF) << 8) | ((config & 0xFF00) >> 8);  // inversion msb lsb
    deviceI2C->WriteReg16(REG_CONFIG, config);
}

/*!
    @brief  Gets the raw bus voltage (float)
*/

float ina219::obtenirTension_V(){

    float vb;
    unsigned short data;

    data = deviceI2C->ReadReg16(REG_BUS);
    data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8); // inversion msb lsb
    //printf("%d\n",data >> 3); // à décommenter pour  évaluer le quantum expérimentalement
    vb = (data >> 3) * quantum/1000.0;  // decalage de 3 bits vers la droite 4.000 mv par bit
    return vb;

}


/*!
    @brief  Gets the current value in mA (float)
*/

float ina219::obtenirCourant_mA(){

    short data;

    data = deviceI2C->ReadReg16(REG_CURRENT);
    data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8); // inversion msb lsb
    return (float)data * 100E-3;   // 100 micro Ampere par bit

}


/*!
    @brief  Gets the current power in Watt (float)
*/

float ina219::obtenirPuissance_W(){

    short data;

    data = deviceI2C->ReadReg16(REG_POWER);
    data = ((data & 0x00FF) << 8) | ((data & 0xFF00) >> 8); // inversion msb lsb
    return (float)data * 2E-3;     // 2 mW par bit

}
