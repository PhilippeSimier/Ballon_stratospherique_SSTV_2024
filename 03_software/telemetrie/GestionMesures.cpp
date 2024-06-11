/*
 * File:   GestionMesures.cpp
 * Author: dbrochard
 *
 * Created on 27 mars 2024, 15:01
 */
 
#include "GestionMesures.h"

GestionMesures::GestionMesures() :
    bme280(ADDRESS_BME280),
    mpu6050(ADDRESS_MPU6050)
{
    // Initialisation de la sensibilité de l'accéléromètre MPU6050
    mpu6050.setAccSensibility(FS_2G);

    // Ouverture du fichier CSV et écriture de l'en-tête
    std::ofstream fichier(CSV_PATH);
    if (fichier.is_open())
    {
        fichier << "Date time,Température_BME,Température_LM,Température_MPU,Pression,Humidité,Accélération_Z" << std::endl;
    }
    else
    {
        throw std::runtime_error("Impossible d'ouvrir le fichier pour enregistrer les en-têtes du fichier CSV.");
    }
    fichier.close();
}

GestionMesures::~GestionMesures()
{
}

void GestionMesures::effectuerMesures()
{
    // Obtention des mesures des capteurs
    mesuresNonFormatees.accelerationVerticale = mpu6050.getAccelZ();
    mesuresNonFormatees.tempMpu = mpu6050.getTemperature();
    mesuresNonFormatees.tempLm = lm75.getTemperature();
    mesuresNonFormatees.tempBme = bme280.obtenirTemperatureEnC();
    mesuresNonFormatees.pression = bme280.obtenirPression();
    mesuresNonFormatees.humidite = bme280.obtenirHumidite();
}

bool GestionMesures::verifierMesures()
{
    bool valid = true; // Variable pour indiquer si les mesures sont valides

    // Vérification de la validité des mesures
    if (mesuresNonFormatees.pression <= VAL_MIN_PRESSION || mesuresNonFormatees.pression >= VAL_MAX_PRESSION)
    {
        valid = false;
        std::cerr << "La pression est invalide  : " << mesuresNonFormatees.pression << " hPa" << std::endl;
        mesuresNonFormatees.pression = NAN;
    }
    if (mesuresNonFormatees.humidite <= VAL_MIN_HUMIDITE || mesuresNonFormatees.humidite >= VAL_MAX_HUMIDITE)
    {
        valid = false;
        std::cerr << "L'humidité est invalide : " << mesuresNonFormatees.humidite << " %" << std::endl;
        mesuresNonFormatees.humidite = NAN;
    }
    if (mesuresNonFormatees.tempBme <= VAL_MIN_TEMPERATURE || mesuresNonFormatees.tempBme >= VAL_MAX_TEMPERATURE)
    {
        valid = false;
        std::cerr << "La temperature est invalide : " << mesuresNonFormatees.tempBme << " °C" << std::endl;
        mesuresNonFormatees.tempBme = NAN;
    }
    if (mesuresNonFormatees.tempLm <= VAL_MIN_TEMPERATURE || mesuresNonFormatees.tempLm >= VAL_MAX_TEMPERATURE)
    {
        valid = false;
        std::cerr << "La temperature est invalide : " << mesuresNonFormatees.tempLm << " °C" << std::endl;
        mesuresNonFormatees.tempLm = NAN;
    }
    if (mesuresNonFormatees.tempMpu <= VAL_MIN_TEMPERATURE || mesuresNonFormatees.tempMpu >= VAL_MAX_TEMPERATURE)
    {
        valid = false;
        std::cerr << "La temperature est invalide : " << mesuresNonFormatees.tempMpu << " °C" << std::endl;
        mesuresNonFormatees.tempMpu = NAN;
    }
    if (mesuresNonFormatees.accelerationVerticale <= VAL_MIN_ACCELERATION || mesuresNonFormatees.accelerationVerticale >= VAL_MAX_ACCELERATION)
    {
        valid = false;
        std::cerr << "L'acceleration verticale est invalide : " << mesuresNonFormatees.accelerationVerticale << " g" << std::endl;
        mesuresNonFormatees.accelerationVerticale = NAN;
    }

    return valid; // Retourne vrai si toutes les mesures sont valides, sinon faux
}

std::string GestionMesures::formaterMesuresPourLora()
{
    effectuerMesures(); // Effectue les mesures

    // Stockage des mesures dans des variables locales
    double t_double = mesuresNonFormatees.tempBme;
    double h_double = mesuresNonFormatees.humidite;
    double p_double = mesuresNonFormatees.pression;
    double a_double = mesuresNonFormatees.accelerationVerticale;

    // Conversion des mesures en chaînes de caractères formatées
    int t_int = static_cast<int>((t_double * 9.0 / 5.0) + 32.0);
    std::string t_str = std::to_string(t_int);
    if (t_int < 100 && t_int >= 10)
    {
        t_str = "0" + t_str;
    }
    else if (t_int < 10 && t_int >= 0)
    {
        t_str = "00" + t_str;
    }
    else if (t_int < 0 && t_int >= -10)
    {
        t_str = "-0" + t_str.erase(0, 1);
    }
    else if (t_int < -10 && t_int > -100)
    {
        t_str = "-" + t_str.erase(0, 1);
    }

    int h_int = static_cast<int>(h_double + 0.5);
    std::string h_str = std::to_string(h_int);
    if (h_int < 10)
    {
        h_str = "0" + h_str;
    }

    int p_int = static_cast<int>((p_double + 0.5) * 10);
    std::string p_str = std::to_string(p_int);
    if (p_int < 10000)
    {
        p_str = "0" + p_str;
    }
    else if (p_int < 1000)
    {
        p_str = "00" + p_str;
    }

    // Construction du message formaté pour LoRa
    std::string messageStr = "_" + gestionTemps.getDateMois() + gestionTemps.getDateJour() + gestionTemps.getDateHeure() + gestionTemps.getDateMinute() + "c...s...g...t" + t_str + "h" + h_str + "b" + p_str + " " + std::to_string(a_double); // \:F4KMN____:test
    return messageStr;
}

void GestionMesures::sauvegarderMesures()
{
    // Ouverture du fichier CSV pour ajouter des données
    std::ofstream fichier(CSV_PATH, std::ios_base::app);
    if (fichier.is_open())
    {
        // Création de l'horodatage
        gestionTemps.majDate();
        fichier << gestionTemps.getDateFormatee();

        // Écriture des mesures dans le fichier CSV
        fichier << setfill('0') << fixed << setprecision(2) << "," << mesuresNonFormatees.tempBme << "," << mesuresNonFormatees.tempLm << "," << mesuresNonFormatees.tempMpu;
        fichier << "," << mesuresNonFormatees.pression << "," << mesuresNonFormatees.humidite << "," << mesuresNonFormatees.accelerationVerticale << std::endl;

        fichier.close();
    }
    else
    {
        throw std::runtime_error("Impossible d'ouvrir le fichier pour enregistrer les mesures.");
    }
}
