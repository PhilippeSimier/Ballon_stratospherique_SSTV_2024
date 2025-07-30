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
        }else{
            oss << "  G";
        }
    } else {
        oss << '!' << latitude_APRS() << symboleTable << longitude_APRS() << symbole; 
         if (alt > 0) {
             oss << "/A=" << std::setw(6) << std::setfill('0') << alt << " ";
         } 
    }
    oss << comment;  
    return oss.str();;
}

std::string Position::latitude_APRS() {
    char hemi; // Hemisphere: N or S
    int ideg; // Degrés entiers
    double dmin; // Minutes décimales
    float latAbs = 0.0;

    if (latitudeDec < 0.0) {
        latAbs = -latitudeDec;
        hemi = 'S';
    } else {
        latAbs = latitudeDec;
        hemi = 'N';
    }

    ideg = static_cast<int> (latAbs);
    dmin = (latAbs - ideg) * 60.0;

    // Arrondi pour éviter que 59.99 devienne 60.00
    if (std::round(dmin * 100) >= 6000) {
        dmin = 0.0;
        ideg++;
    }

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << ideg
            << std::fixed << std::setprecision(2) << std::setw(5) << dmin
            << hemi;

    return oss.str();
}

std::string Position::longitude_APRS() {
    char hemi; // Hemisphere: W or E
    int ideg; // Degrés entiers
    double dmin; // Minutes décimales
    float longAbs = 0.0;

    if (longitudeDec < 0.0) {
        longAbs = -longitudeDec;
        hemi = 'W';
    } else {
        longAbs = longitudeDec;
        hemi = 'E';
    }

    ideg = static_cast<int> (longAbs);
    dmin = (longAbs - ideg) * 60.0;

    // Correction pour éviter d’afficher 60.00 minutes
    if (std::round(dmin * 100) >= 6000) {
        dmin = 0.0;
        ideg++;
    }

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << ideg
            << std::fixed << std::setprecision(2) << std::setw(5) << dmin
            << hemi;

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

