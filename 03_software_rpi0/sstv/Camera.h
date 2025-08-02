/* 
 * File:   Camera.h
 * Author: Philippe Simier
 *
 * Created on 21 mars 2024, 09:10
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <sstream>
#include <string>

class Camera {
public:
    
    Camera(const unsigned long _frequence, const std::string _indicatif);
    Camera(const Camera& orig);
    virtual ~Camera();
    
    void envoyerPhoto();
    void enregistrerPhoto();

private:
    unsigned long frequence;
    const std::string indicatif;
    int i;

};

#endif /* CAMERA_H */

