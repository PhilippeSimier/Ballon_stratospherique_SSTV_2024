/* 
 * File:   Position.cpp
 * Author: philippe SIMIER (F4JRE)
 * 
 * Created on 23 août 2025, 16:11
 */

#include "Position.h"

Position::Position(double lat, double lon, double alt, char table, char _symbole, std::string _comment) :
latitude(lat),
longitude(lon),
altitude(alt),
symboleTable(table),
symbole(_symbole),
comment(_comment)
{

}

Position::Position(const Position& orig) {
}

Position::~Position() {
}

// Assesseurs

double Position::getLatitude() const {
    return latitude;
}

double Position::getLongitude() const {
    return longitude;
}

double Position::getAltitude() const {
    return altitude;
}

double Position::deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

double Position::distanceTo(const Position& other) const {

    // Conversion en radians
    double lat1 = deg2rad(latitude);
    double lon1 = deg2rad(longitude);
    double lat2 = deg2rad(other.latitude);
    double lon2 = deg2rad(other.longitude);

    // Haversine
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
            std::cos(lat1) * std::cos(lat2) *
            std::sin(dlon / 2) * std::sin(dlon / 2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    double distance2D = R * c;

    // Différence altitude
    double dalt = other.altitude - altitude;

    return std::sqrt(distance2D * distance2D + dalt * dalt);

}

int Position::save(std::string chemin) {

    std::ofstream fichier(chemin); // ouvre (ou crée) le fichier

    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier !" << std::endl;
        return 1;
    }

    // Écrire les trois valeurs séparées par un espace
    fichier << std::fixed << std::setprecision(5);
    fichier << latitude << " " << longitude << " " << std::setprecision(1) << altitude << std::endl;

    fichier.close();
    return 0;
}

void Position::set(double lat, double lon, double alt) {
    latitude = lat;
    longitude = lon;
    altitude = alt;
}

std::string Position::getLocator(int nbChar) {


    float lon = longitude + 180.0f;
    float lat = latitude + 90.0f;

    char locator[9];

    // Field (A-R)
    locator[0] = 'A' + static_cast<int> (lon / 20);
    locator[1] = 'A' + static_cast<int> (lat / 10);

    // Square (0-9)
    locator[2] = '0' + static_cast<int> (fmod(lon, 20) / 2);
    locator[3] = '0' + static_cast<int> (fmod(lat, 10) / 1);

    // Subsquare (A-X)
    locator[4] = 'A' + static_cast<int> (fmod(lon, 2) * 12);
    locator[5] = 'A' + static_cast<int> (fmod(lat, 1) * 24);

    // Extended square (0-9)
    locator[6] = '0' + static_cast<int> (fmod(lon * 120, 10));
    locator[7] = '0' + static_cast<int> (fmod(lat * 240, 10));

    locator[8] = '\0';

    // Clamp nbChar
    if (nbChar < 2) nbChar = 2;
    if (nbChar > 8) nbChar = 8;
    if (nbChar % 2 != 0) nbChar--; // Ensure even number

    return std::string(locator, nbChar);
}

std::string Position::latitude_APRS() {

    char hemi = (latitude >= 0) ? 'N' : 'S';
    double absLat = std::fabs(latitude);
    int deg = static_cast<int> (absLat);
    double min = (absLat - deg) * 60.0;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << deg;
    oss << std::setw(5) << std::setfill('0') << std::fixed << std::setprecision(2) << min << hemi;

    return oss.str();
}

std::string Position::longitude_APRS() {

    char hemi = (longitude >= 0) ? 'E' : 'W';
    double absLon = std::fabs(longitude);
    int deg = static_cast<int> (absLon);
    double min = (absLon - deg) * 60.0;

    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << deg;
    oss << std::setw(5) << std::setfill('0') << std::fixed << std::setprecision(2) << min << hemi;

    return oss.str();
}

std::string Position::latitude_APRS_comp() {

    int y;
    y = static_cast<int> (380926. * (90. - latitude));
    return convBase91(y);
}

std::string Position::longitude_APRS_comp() {

    int x;
    x = static_cast<int> (190463. * (180. + longitude));
    return convBase91(x);
}

std::string Position::convBase91(int x) {
    std::string base91(4, ' '); // Initialise une string de 4 caractères

    for (int i = 3; i >= 0; i--) {
        base91[i] = (x % 91) + 33;
        x /= 91;
    }

    return base91;
}

/**
 * Méthode pour compresser l'altitude 
 */
std::string Position::altitude_APRS_comp() {

    int altFeet = round(3.2809 * altitude);
    int x = static_cast<int> (log(altFeet) / 0.001998003);
    std::string alt91(3, ' '); // Initialise une string de 3 caractères
    alt91[0] = (x / 91) + 33;
    alt91[1] = (x % 91) + 33;
    alt91[2] = 'S';
    return alt91;

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
        if (altitude > 0) {
            oss << altitude_APRS_comp();
        } else {
            oss << "  G";
        }
    } else {
        oss << '!' << latitude_APRS() << symboleTable << longitude_APRS() << symbole;
        if (altitude > 0) {
            int altFeet = round(3.2809 * altitude);
            oss << "/A=" << std::setw(6) << std::setfill('0') << altFeet << " ";
        }
    }
    oss << comment;
    return oss.str();
}