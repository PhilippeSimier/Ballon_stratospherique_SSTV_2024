/* 
 * File:   Camera.h
 * Author: Philippe Simier
 *
 * Created on 21 mars 2024, 09:10
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <string>

class Camera {
public:
    Camera();
    Camera(const Camera& orig);
    virtual ~Camera();
    
    static void envoyerPhoto(const unsigned long frequence = 28680000);
    static void enregistrerPhoto(int i);
private:

};

#endif /* CAMERA_H */

