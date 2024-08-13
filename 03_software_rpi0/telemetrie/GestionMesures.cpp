/*
 * File:   GestionMesures.cpp
 * Authors: Damien Brochard, Philippe Simier
 *
 * Created on 27 mars 2024, 15:01
 */
 
#include "GestionMesures.h"

GestionMesures::GestionMesures() :
    bme280(ADDRESS_BME280)
{
    mpu.begin(ADDRESS_MPU6050);
    // Initialisation de la sensibilité de l'accéléromètre MPU6050
    mpu.setAccSensibility(MPU6050::FS_2G);
    // Initialisation des offsets to do

    // Ouverture du fichier CSV et écriture de l'en-tête
    std::ofstream fichier(CSV_PATH);
    if (fichier.is_open())
    {
        fichier << "Date Time Time_Zone,Température_BME,Température_LM,Température_MPU,Pression,Humidité,Accel_X,Accel_Y,Accel_Z" << std::endl;

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
    mesures.accelX  = mpu.getAccelX();
    mesures.accelY  = mpu.getAccelY();
    mesures.accelZ  = mpu.getAccelZ();
    mesures.tempMpu = mpu.getTemperature();
    mesures.tempLm  = lm75.getTemperature();
    mesures.tempBme = bme280.obtenirTemperatureEnC();
    mesures.pression = bme280.obtenirPression();
    mesures.humidite = bme280.obtenirHumidite();
}

bool GestionMesures::verifierMesures()
{
    bool valid = true; // Variable pour indiquer si les mesures sont valides

    // Vérification de la validité des mesures
    if (mesures.pression <= VAL_MIN_PRESSION || mesures.pression >= VAL_MAX_PRESSION)
    {
        valid = false;
        mesures.pression = NAN;
    }
    if (mesures.humidite <= VAL_MIN_HUMIDITE || mesures.humidite >= VAL_MAX_HUMIDITE)
    {
        valid = false;
        mesures.humidite = NAN;
    }
    if (mesures.tempBme <= VAL_MIN_TEMPERATURE || mesures.tempBme >= VAL_MAX_TEMPERATURE)
    {
        valid = false;
        mesures.tempBme = NAN;
    }
    if (mesures.tempLm <= VAL_MIN_TEMPERATURE || mesures.tempLm >= VAL_MAX_TEMPERATURE)
    {
        valid = false;
        std::cerr << "La temperature est invalide : " << mesures.tempLm << " °C" << std::endl;
        mesures.tempLm = NAN;
    }
    if (mesures.tempMpu <= VAL_MIN_TEMPERATURE || mesures.tempMpu >= VAL_MAX_TEMPERATURE)
    {
        valid = false;
        mesures.tempMpu = NAN;
    }
    if (mesures.accelX <= VAL_MIN_ACCELERATION || mesures.accelX >= VAL_MAX_ACCELERATION)
    {
        valid = false;
        mesures.accelX = NAN;
    }
    if (mesures.accelY <= VAL_MIN_ACCELERATION || mesures.accelY >= VAL_MAX_ACCELERATION)
    {
        valid = false;
        mesures.accelY = NAN;
    }
    if (mesures.accelZ <= VAL_MIN_ACCELERATION || mesures.accelZ >= VAL_MAX_ACCELERATION)
    {
        valid = false;
        mesures.accelZ = NAN;
    }

    return valid; // Retourne vrai si toutes les mesures sont valides, sinon faux
}

/**
 * @brief GestionMesures::formaterMesuresPourLora
 * @return une trame au format aprs weather avec pour commentaire
 *         les accelérations X,Y,Z
 */
std::string GestionMesures::formaterMesuresPourLora()
{
    effectuerMesures();

    // Conversion des mesures en valeur entière
    int t = static_cast<int>((mesures.tempBme * 9.0 / 5.0) + 32.0);
    int h = static_cast<int>(mesures.humidite + 0.5);
    int p = static_cast<int>((mesures.pression + 0.5) * 10);

    // Construction de la trame aprs weather
    std::string trameAprsWeather = "_" + gestionTemps.getDateAprs();
    trameAprsWeather += "c...s...g...t" + intToString(t, 3);
    trameAprsWeather += "h" + intToString(h, 2);
    trameAprsWeather += "b" + intToString(p, 4);
    trameAprsWeather += " " + doubleToString(mesures.accelX);
    trameAprsWeather += " " + doubleToString(mesures.accelY);
    trameAprsWeather += " " + doubleToString(mesures.accelZ);

    return trameAprsWeather;
}

/**
 * @brief GestionMesures::sauvegarderMesures
 * @abstract ajoute une ligne de mesures dans le fichier CSV
 */
void GestionMesures::sauvegarderMesures()
{
    // Ouverture du fichier CSV pour ajouter des données
    std::ofstream fichier(CSV_PATH, std::ios_base::app);
    if (fichier.is_open())
    {
        std::ostringstream out;

        out << setfill('0') << fixed << setprecision(2)  << mesures.tempBme << "," << mesures.tempLm << "," << mesures.tempMpu;
        out << "," << mesures.pression << "," << mesures.humidite;
        out << "," << mesures.accelX << "," << mesures.accelY << "," << mesures.accelZ << std::endl;

        // Création de l'horodatage
        fichier << gestionTemps.getDateFormatee();
        fichier << ",";
        // Écriture des mesures dans le fichier CSV et sur la sortie cout
        fichier << out.str();
        std::cout << out.str();
        fichier.close();


    }
    else
    {
        throw std::runtime_error("Impossible d'ouvrir le fichier pour enregistrer les mesures.");
    }
}

/**
 * @brief GestionMesures::doubleToString
 * @param value
 * @return la valeur représentée en string avec 2 chiffres derrière la virgule
 */
std::string GestionMesures::doubleToString(double value) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << value;
    return out.str();
}

/**
 * @brief GestionMesures::intToString
 * @param value un entier
 * @param format le nombre de chiffres significatifs
 * @return  la valeur représentée en string formatée avec le nb de chiffres
 */
std::string GestionMesures::intToString(int value, int format) {
    std::ostringstream oss;
    // Configure le flux pour remplir avec des zéros et avoir une largeur correspondant à format
    oss << std::setw(format) << std::setfill('0') << value;
    return oss.str();
}
