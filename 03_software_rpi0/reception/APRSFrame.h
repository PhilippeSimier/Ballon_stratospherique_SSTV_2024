/* 
 * File:   APRSFrame.h
 * Author: philippe
 *
 * Created on 1 août 2025, 08:10
 */

#ifndef APRSFRAME_H
#define APRSFRAME_H

#include <iostream>
#include <string>

class APRSFrame {
public:

    enum class FrameType {
        Message,
        Position,
        Status,
        Telemetry,
        Weather,
        Unknown
    };

    APRSFrame();
    APRSFrame(const std::string& frame);
    APRSFrame(const APRSFrame& orig);
    virtual ~APRSFrame();
    
    void setRaw(const std::string& frame);
    std::string getSource() const; 
    std::string getDestination() const; 
    std::string getPath() const; 
    
    std::string getAddressee() const; 
    std::string getMessage() const; 
    
    double getLatitude() const;
    double getLongitude() const; 
    
    FrameType getFrameType() const; 
    
    
    void print() const;
    
    static std::string typeToString(FrameType type);
    static double parseCoordinate(const std::string& coord, char direction);
    static void rtrim(std::string &s);

private:
    void parse();
    void parsePosition(std::string payload);

    std::string rawFrame;
    FrameType type;
    std::string source;
    std::string destination; // Groupe
    std::string path;
    
    std::string addressee; // Destinataire message
    std::string message;
    
    double latitude = 0.0;   
    double longitude = 0.0;  
    bool hasPosition = false; 

};

#endif /* APRSFRAME_H */

