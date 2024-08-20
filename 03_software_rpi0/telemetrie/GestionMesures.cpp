/*
 * File:   GestionMesures.cpp
 * Authors: Damien Brochard, Philippe Simier
 *
 * Created on 27 mars 2024, 15:01
 * Improve on 16 august 2024
 */
 
#include "GestionMesures.h"

GestionMesures::GestionMesures() :
    bme280(ADDRESS_BME280)
{
    mpu.begin(ADDRESS_MPU6050);

    // Affectation de la locale personnalisée à localeAvecVirgule
    localeAvecVirgule = std::locale(std::locale(), new VirguleDecimal);

    ini.Load("/home/ballon/telemetrie/config_MPU6050.ini");

    // Initialisation de la sensibilité du MPU6050
    string rangeAcc = ini.GetValue("MPU6050", "rangeAcc", "FS_2G");
    string rangeGyro = ini.GetValue("MPU6050", "rangeGyro", "FS_250DPS");
    mpu.setAccSensibility(mpu.sensibilityFromString(rangeAcc));
    mpu.setGyroSensibility(mpu.sensibilityFromString(rangeGyro));
    cout << "sensibilité : " << rangeAcc << " : " << rangeGyro << endl;

    // Initialisation des offsets   
    int16_t oax = ini.GetValue<int16_t>("MPU6050", "offsetAX", 0);
    int16_t oay = ini.GetValue<int16_t>("MPU6050", "offsetAY", 0);
    int16_t oaz = ini.GetValue<int16_t>("MPU6050", "offsetAZ", 0);
    cout << "offset Acc : " << oax << " : " << oay << " : " << oaz << endl;
    mpu.setAccelOffset(oax,oay,oaz);
    int16_t ogx = ini.GetValue<int16_t>("MPU6050", "offsetGX", 0);
    int16_t ogy = ini.GetValue<int16_t>("MPU6050", "offsetGY", 0);
    int16_t ogz = ini.GetValue<int16_t>("MPU6050", "offsetGZ", 0);
    cout << "offset Gyro : " << ogx << " : " << ogy << " : " << ogz << endl;
    mpu.setGyroOffset(ogx,ogy,ogz);

    // Ouverture du fichier CSV et écriture de l'en-tête
    std::ofstream fichier(CSV_PATH);
    if (fichier.is_open())
    {
        fichier << "Date Time Time_Zone;Température_BME;Température_LM;Température_MPU;Pression;Humidité;Accel_X;Accel_Y;Accel_Z;Gyro_X;Gyro_Y;Gyro_Z" << std::endl;

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
    mesures.giroX   = mpu.getRotationX();
    mesures.giroY   = mpu.getRotationY();
    mesures.giroZ   = mpu.getRotationZ();
    mesures.tempMpu = mpu.getTemperature();
    mesures.tempLm  = lm75.getTemperature();
    mesures.tempBme = bme280.obtenirTemperatureEnC();
    mesures.pression = bme280.obtenirPression();
    mesures.humidite = bme280.obtenirHumidite();

    verifierMesures();
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
    if ((mesures.giroX <= 0.2) && (mesures.giroX >= -0.2)){
        mesures.giroX = 0.0;
    }
    if ((mesures.giroY <= 0.2) && (mesures.giroY >= -0.2)){
        mesures.giroY = 0.0;
    }
    if ((mesures.giroZ <= 0.2) && (mesures.giroZ >= -0.2)){
        mesures.giroZ = 0.0;
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
    std::ostringstream out;
    out.imbue(localeAvecVirgule); // Appliquer la locale personnalisée au flux
    out << setfill('0') << fixed;
    out << "_" << gestionTemps.getDateAprs();
    out << "c...s...g...t" << std::setw(3) << t;
    out << "h" << std::setw(2) << h;
    out << "b" << std::setw(4) << p;
    out << showpos;
    out << " " << setprecision(2) << mesures.accelX << " " << mesures.accelY << " " << mesures.accelZ;
    out << " " << setprecision(1) << mesures.giroX << " " << mesures.giroY << " " << mesures.giroZ;

    return out.str();
}

/**
 * @brief GestionMesures::sauvegarderMesures
 * @abstract ajoute une ligne de mesures dans le fichier CSV
 */
void GestionMesures::sauvegarderMesures()
{

    effectuerMesures();

    // Ouverture du fichier CSV pour ajouter des données
    std::ofstream fichier(CSV_PATH, std::ios_base::app);
    if (fichier.is_open())
    {
        std::ostringstream out;
        out.imbue(localeAvecVirgule); // Appliquer la locale personnalisée au flux

        out << setfill('0') << fixed << setprecision(2)  << mesures.tempBme << ";" << mesures.tempLm << ";" << mesures.tempMpu;
        out << ";" << mesures.pression << ";" << mesures.humidite;
        out << showpos;  // affiche le signe +
        out << ";" << mesures.accelX << ";" << mesures.accelY << ";" << mesures.accelZ;
        out << ";" << setprecision(1) << mesures.giroX << ";" << mesures.giroY << ";" << mesures.giroZ << std::endl;
        // Création de l'horodatage
        fichier << gestionTemps.getDateFormatee();
        fichier << ";";
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


