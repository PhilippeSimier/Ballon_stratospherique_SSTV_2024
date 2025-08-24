/* 
 * File:   Position.h
 * Author: philippe Simier (F4JRE)
 *
 * Created on 23 août 2025, 16:11
 */

#ifndef POSITION_H
#define POSITION_H
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>

class Position {
public:

    Position(double lat, 
             double lon, 
             double alt = 0.0, 
             char table = '/', 
             char symbole = '>',
             std::string _comment = "");
    
    Position(const Position& orig);
    virtual ~Position();

    // Accesseurs
    double getLatitude() const;
    double getLongitude() const;
    double getAltitude() const;

    int save(std::string chemin);
    void set(double lat, double lon, double alt = 0.0);

    // Calcul de la distance entre deux positions (mètres)
    double distanceTo(const Position& other) const;

    // Obtenir le locator
    std::string getLocator(int nbChar = 8);
    
    // Obtenir la trame position APRS
    std::string getPduAprs(bool compressed = false);

private:

    double latitude; // degrés
    double longitude; // degrés
    double altitude; // mètres
    char symboleTable;
    char symbole;
    std::string comment;

    static constexpr double R = 6371000.0; // rayon moyen de la Terre en m
    static double deg2rad(double deg);
    static std::string convBase91(int x);
    
    std::string latitude_APRS();
    std::string longitude_APRS();
    std::string latitude_APRS_comp();
    std::string longitude_APRS_comp();
    std::string altitude_APRS_comp();
    

};

#endif /* POSITION_H */

