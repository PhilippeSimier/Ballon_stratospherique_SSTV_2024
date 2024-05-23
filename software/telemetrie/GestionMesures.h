#ifndef GESTIONMESURES_H
#define GESTIONMESURES_H

#include "MPU6050.h"
#include "LM75.h"
#include "BME280.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <chrono>
#include <thread>

namespace
{
    constexpr char CSV_PATH[] = "/home/ballon/data.csv";

    constexpr int ADDRESS_BME280 = 0x77;
    constexpr int ADDRESS_MPU6050 = 0x69;

    constexpr double VAL_MIN_TEMPERATURE = -30.0;
    constexpr double VAL_MAX_TEMPERATURE = 120.0;
    constexpr double VAL_MIN_PRESSION = 0.0;
    constexpr double VAL_MAX_PRESSION = 1100.0;
    constexpr double VAL_MIN_HUMIDITE = 0.0;
    constexpr double VAL_MAX_HUMIDITE = 100.0;
    constexpr double VAL_MIN_ACCELERATION = -10.0;
    constexpr double VAL_MAX_ACCELERATION = 10.0;
}

struct Temps
{
    std::string annee{};
    std::string mois{};
    std::string jour{};
    std::string heure{};
    std::string minute{};
    std::string seconde{};
};

struct MesuresNonFormatees
{
    double tempMpu;
    double tempLm;
    double tempBme;
    double pression;
    double humidite;
    double accelerationVerticale;
};

struct MesuresFormateesPourLora
{
    double tempMpu;
    double tempLm;
    double tempBme;
    double pression;
    double humidite;
    double accelerationVerticale;
};

class GestionMesures
{

public:
    GestionMesures();
    ~GestionMesures();

    void effectuerMesures();
    bool verifierMesures();
    std::string formaterMesuresPourLora();
    void sauvegarderMesures();
    void majDate();

    double getTemperatureMpu();
    double getTemperatureLm();
    double getTemperatureBme();
    double getPression();
    double getHumidite();
    double getAccelerationVerticale();

    std::string getDateAnnee() const;
    std::string getDateMois() const;
    std::string getDateJour() const;
    std::string getDateHeure() const;
    std::string getDateMinute() const;
    std::string getDateSeconde() const;

private:
    Temps temps;

    MesuresNonFormatees mesuresNonFormatees;
    MesuresFormateesPourLora mesuresFormateesPourLora;

    BME280 bme280;
    MPU6050 mpu6050;
    LM75 lm75;
};

#endif /* GESTIONMESURES_H */
