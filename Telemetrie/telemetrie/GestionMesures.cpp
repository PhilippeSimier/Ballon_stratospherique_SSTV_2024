#include "GestionMesures.h"

GestionMesures::GestionMesures() : 
bme280(ADDRESS_BME280),
mpu6050(0x69)        
{

    mpu6050.setAccSensibility(FS_2G);
    std::ofstream fichier("/home/ballon/data.csv");
    if (fichier.is_open())
    {
        fichier << "Date time,Température BME, Température LM, Température MPU, Pression,Humidité,Accélération Z" <<  std::endl;
    }
    fichier.close();
}

GestionMesures::~GestionMesures()
{
}

void GestionMesures::effectuerMesures()
{

    accelerationVerticale = mpu6050.getAccelZ();
    tempMpu = mpu6050.getTemperature();
    tempLm = lm75.getTemperature();
    tempBme = bme280.obtenirTemperatureEnC();
   
    pression = bme280.obtenirPression();
    humidite = bme280.obtenirHumidite();
}

bool GestionMesures::verifierMesures()
{
    bool valid = true;
    if (pression <= VAL_MIN_PRESSION || pression >= VAL_MAX_PRESSION)
    {
        valid = false;
        std::cerr << "La pression est invalide  : " << pression << " hPa" << std::endl;
        pression = NAN; 
    }
    if (humidite <= VAL_MIN_HUMIDITE || humidite >= VAL_MAX_HUMIDITE)
    {
        valid = false;
        std::cerr << "L'humidité est invalide : " << humidite << " %" << std::endl;
        humidite = NAN;
    }
    if (tempBme <= VAL_MIN_TEMPERATURE || tempBme >= VAL_MAX_TEMPERATURE)
    {
        valid = false;
        std::cerr << "La temperature est invalide : " << tempBme << " °C" << std::endl;
        tempBme = NAN;
    }
    if (tempLm <= VAL_MIN_TEMPERATURE || tempLm >= VAL_MAX_TEMPERATURE)
    {
        valid = false;
        std::cerr << "La temperature est invalide : " << tempLm << " °C" << std::endl;
        tempLm = NAN;
    }
    if (tempMpu <= VAL_MIN_TEMPERATURE || tempMpu >= VAL_MAX_TEMPERATURE)
    {
        valid = false;
        std::cerr << "La temperature est invalide : " << tempMpu << " °C" << std::endl;
        tempMpu = NAN;
    }
    if (accelerationVerticale <= VAL_MIN_ACCELERATION || accelerationVerticale >= VAL_MAX_ACCELERATION)
    {
        valid = false;
        std::cerr << "L'acceleration verticale est invalide : " << accelerationVerticale << " g" << std::endl;
        accelerationVerticale = NAN;
    }

    return valid;
}

void GestionMesures::sauvegarderMesures()
{
    std::ofstream fichier("/home/ballon/data.csv", std::ios_base::app);
    if (fichier.is_open())
    {
        std::time_t tempsActuel = std::time(nullptr);
        std::tm *tempsLocal = std::localtime(&tempsActuel);
        char timeStamp[20];
        std::strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%d %H:%M:%S", tempsLocal);
        fichier << timeStamp;
        fichier << setfill('0') << fixed << setprecision(2)  << "," << tempBme;
        fichier << "," << tempLm << "," << tempMpu;
        fichier << "," << pression << "," << humidite << "," << accelerationVerticale << std::endl;
        fichier.close();
    }
    else
    {
        std::cerr << "Impossible d'ouvrir le fichier pour enregistrer les " << std::endl;
    }
}



double GestionMesures::getTemperatureMpu()
{
    return tempMpu;
}

double GestionMesures::getTemperatureLm()
{
    return tempLm;
}

double GestionMesures::getTemperatureBme()
{
    return tempBme;
}

double GestionMesures::getPression()
{
    return pression;
}

double GestionMesures::getHumidite()
{
    return humidite;
}

double GestionMesures::getAccelerationVerticale()
{
    return accelerationVerticale;
}

