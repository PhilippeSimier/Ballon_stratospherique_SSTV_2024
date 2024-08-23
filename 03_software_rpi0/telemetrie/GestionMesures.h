/*
 * File:   GestionMesures.h
 * Author: dbrochard
 *
 * Created on 27 mars 2024, 15:01
 */

#ifndef GESTIONMESURES_H
#define GESTIONMESURES_H

#include "MPU6050.h"
#include "LM75.h"
#include "BME280.h"
#include "GestionTemps.h"
#include "SimpleIni.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <locale>

#include <stdexcept>
#include <chrono>
#include <thread>

namespace
{


constexpr int ADDRESS_BME280 = 0x77; // Adresse du capteur BME280
constexpr int ADDRESS_MPU6050 = 0x69; // Adresse du capteur MPU6050

// Définition des valeurs minimales et maximales pour les mesures
constexpr double VAL_MIN_TEMPERATURE = -30.0;
constexpr double VAL_MAX_TEMPERATURE = 120.0;
constexpr double VAL_MIN_PRESSION = 0.0;
constexpr double VAL_MAX_PRESSION = 1100.0;
constexpr double VAL_MIN_HUMIDITE = 0.0;
constexpr double VAL_MAX_HUMIDITE = 100.0;
constexpr double VAL_MIN_ACCELERATION = -10.0;
constexpr double VAL_MAX_ACCELERATION = 10.0;
}

// Structure pour stocker les mesures non formatées
struct Mesures
{
    double tempMpu;
    double tempLm;
    double tempBme;
    double pression;
    double humidite;
    double accelX;
    double accelY;
    double accelZ;
    double giroX;
    double giroY;
    double giroZ;
};

// Création d'une classe personnalisée dérivée de std::numpunct
class VirguleDecimal : public std::numpunct<char> {
protected:
    // Surcharge de la fonction do_decimal_point() pour retourner une virgule
    virtual char do_decimal_point() const override {
        return ',';
    }
};

class GestionMesures
{

public:
    GestionMesures(); // Constructeur
    ~GestionMesures(); // Destructeur

    std::string calibrerMPU();      // Méthode pour calibrer le MPU6050
    void effectuerMesures(); // Fonction pour effectuer les mesures
    bool verifierMesures(); // Fonction pour vérifier les mesures
    std::string formaterMesuresPourLora(); // Fonction pour formater les mesures pour LoRa
    void sauvegarderMesures(); // Fonction pour sauvegarder les mesures dans un fichier

    // Fonctions pour obtenir les différentes mesures
    double getTemperatureMpu();
    double getTemperatureLm();
    double getTemperatureBme();
    double getPression();
    double getHumidite();
    double getAccelerationVerticale();


private:
    GestionTemps gestionTemps; // Variable pour stocker le temps

    // Variables pour stocker les mesures
    Mesures mesures;

    // Objets des capteurs
    BME280 bme280;
    LM75 lm75;
    SimpleIni ini;

    std::locale localeAvecVirgule;
    string fichierCSV;


};

#endif /* GESTIONMESURES_H */
