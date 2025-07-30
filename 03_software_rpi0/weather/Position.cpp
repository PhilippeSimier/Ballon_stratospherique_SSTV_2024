/* 
 * File:   Position.cpp
 * Author: philippe SIMIER (F4JRE) 
 * 
 * Created on 7 août 2021, 08:00
 */

#include "Position.h"

/**
 * 
 * @param _latitude       double longitude en degrés décimaux
 * @param _longitude      double longitude en degrés décimaux
 * @param _comment        String le commentaire
 * @param _symboleTable   char pour désigner la table des symboles
 * @param _symbole        char pour désigner un symbole dans la table 
 */
Position::Position(const float _latitude, const float _longitude, std::string _comment, const char _symboleTable, const char _symbole) :
latitudeDec(_latitude),
longitudeDec(_longitude),
symboleTable(_symboleTable),
symbole(_symbole),
alt(0),
comment(_comment) {

}

Position::Position(const Position& orig) {
}

Position::~Position() {
}

void Position::setLatitude(const float _latitude) {
    latitudeDec = _latitude;
}

void Position::setLongitude(const float _longitude) {
    longitudeDec = _longitude;
}

void Position::setComment(std::string _comment) {
    comment = _comment;
}

void Position::setAltitude(const float _alt) {
    alt = round(3.2809 * _alt);
}

/**
 * @brief Fabrique le PDU APRS position 
 *        si compressed est true la position est compressée (base91)
 * @param bool compressed indique si la position doit être compressée
 * @return std::string Le pdu APRS position 
 */
std::string Position::getPduAprs(bool compressed) {

    std::ostringstream oss;
    if (compressed) {
        oss << '!' << symboleTable << latitude_APRS_comp() << longitude_APRS_comp() << symbole;
        if (alt > 0) {
            oss << altitude_APRS_comp();
        } else {
            oss << "  G";
        }
    } else {
        oss << '!' << latitude_APRS() << symboleTable << longitude_APRS() << symbole;
        if (alt > 0) {
            oss << "/A=" << std::setw(6) << std::setfill('0') << alt << " ";
        }
    }
    oss << comment;
    return oss.str();
    ;
}

std::string Position::latitude_APRS() {

    char hemi = (latitudeDec >= 0) ? 'N' : 'S';
    float absLat = abs(latitudeDec);
    int deg = static_cast<int> (absLat);
    float min = (absLat - deg) * 60.0;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << deg;
    oss << std::setw(5) << std::setfill('0') << std::fixed << std::setprecision(2) << min << hemi;

    return oss.str();
}

std::string Position::longitude_APRS() {

    char hemi = (longitudeDec >= 0) ? 'E' : 'W';
    float absLon = abs(longitudeDec);
    int deg = static_cast<int> (absLon);
    float min = (absLon - deg) * 60.0;

    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << deg;
    oss << std::setw(5) << std::setfill('0') << std::fixed << std::setprecision(2) << min << hemi;

    return oss.str();
}

std::string Position::latitude_APRS_comp() {

    int y;
    y = static_cast<int> (380926. * (90. - latitudeDec));
    return convBase91(y);
}

std::string Position::longitude_APRS_comp() {

    int x;
    x = static_cast<int> (190463. * (180. + longitudeDec));
    return convBase91(x);
}

/**
 * Méthode pour compresser l'altitude 
 */
std::string Position::altitude_APRS_comp() {

    int x;
    x = static_cast<int> (log(alt) / 0.001998003);
    std::string alt91(3, ' '); // Initialise une string de 3 caractères
    alt91[0] = (x / 91) + 33;
    alt91[1] = (x % 91) + 33;
    alt91[2] = 'S';
    return alt91;

}

std::string Position::convBase91(int x) {
    std::string base91(4, ' '); // Initialise une string de 4 caractères

    for (int i = 3; i >= 0; i--) {
        base91[i] = (x % 91) + 33;
        x /= 91;
    }

    return base91;
}


std::string Position::getLocator(int nbChar) {
        

        float lon = longitudeDec + 180.0f;
        float lat = latitudeDec + 90.0f;

        char locator[9];

        // Field (A-R)
        locator[0] = 'A' + static_cast<int>(lon / 20);
        locator[1] = 'A' + static_cast<int>(lat / 10);

        // Square (0-9)
        locator[2] = '0' + static_cast<int>(fmod(lon, 20) / 2);
        locator[3] = '0' + static_cast<int>(fmod(lat, 10) / 1);

        // Subsquare (A-X)
        locator[4] = 'A' + static_cast<int>(fmod(lon, 2) * 12);
        locator[5] = 'A' + static_cast<int>(fmod(lat, 1) * 24);

        // Extended square (0-9)
        locator[6] = '0' + static_cast<int>(fmod(lon * 120, 10));
        locator[7] = '0' + static_cast<int>(fmod(lat * 240, 10));

        locator[8] = '\0';

        // Clamp nbChar
        if (nbChar < 2) nbChar = 2;
        if (nbChar > 8) nbChar = 8;
        if (nbChar % 2 != 0) nbChar--; // Ensure even number

        return std::string(locator, nbChar);
    }
