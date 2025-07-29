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
    void setLatitude(const float _latitude);
    void setLongitude(const float _longitude);
    void setComment(std::string _comment);
    void setAltitude(const float _alt);



private:



    std::string slat;
    std::string slong;
    std::string salt;
    float latitude;
    float longitude;
    char symboleTable;
    char symbole;
    std::string pdu;

    int alt;
    std::string comment;
    
    void latitude_to_str();
    void longitude_to_str();
    void latitude_to_comp_str();
    void longitude_to_comp_str();
    void altitude_to_comp_str();
    std::string convBase91(int x);
};

#endif /* POSITION_H */

