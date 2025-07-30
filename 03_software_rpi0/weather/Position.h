/* 
 * File:   Position.h
 * Author: philippe SIMIER (F4JRE)
 *
 * Created on 27 juillet 2025, 18:00
 */

#ifndef POSITION_H
#define POSITION_H

#include <string>
#include <math.h>
#include <sstream>
#include <iomanip>

class Position {
public:

    Position(const float _latitude,
            const float _longitude,
            std::string _comment = "",
            const char _symboleTable = '/',
            const char _symbole = '>'
            );

    Position(const Position& orig);
    virtual ~Position();

    std::string getPduAprs(bool compressed = false);
    std::string getLocator(int nbChar);
    
    void setLatitude(const float _latitude);
    void setLongitude(const float _longitude);
    void setComment(std::string _comment);
    void setAltitude(const float _alt);
    

private:

    float latitudeDec;
    float longitudeDec;
    char symboleTable;
    char symbole;
    int alt;
    std::string comment;
    
    std::string latitude_APRS();
    std::string longitude_APRS();
    std::string latitude_APRS_comp();
    std::string longitude_APRS_comp();
    std::string altitude_APRS_comp();
    std::string convBase91(int x);
};

#endif /* POSITION_H */

