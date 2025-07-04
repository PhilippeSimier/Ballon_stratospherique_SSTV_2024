/* 
 * File:   Camera.cpp
 * Author: Philippe SIMIER Lycée Touchard le Mans
 * 
 * Created on 21 mars 2024, 09:10
 */

#include "Camera.h"

Camera::Camera():
i(0)
{
}

Camera::Camera(const Camera& orig) {
}

Camera::~Camera() {
}

void Camera::envoyerPhoto(const unsigned long frequence){
    
    // Prise d'une photo en basse résolution
    system("libcamera-still --width 320 --height 256 -o /home/ballon/photo.jpg");
    // add texte & date
    system(R"(convert -pointsize 20 -box white -draw "text 10,20 'F4KMN `date +"%d/%m/%Y %T"`'" /home/ballon/photo.jpg /home/ballon/photo_date.jpg)");
    // Convertion en RGB 8 bits
    system(R"(convert -depth 8 /home/ballon/photo_date.jpg /home/ballon/photo_date.rgb)");
    // Emission SSTV
    std::ostringstream commande_rpitx;
    commande_rpitx << "sudo /home/ballon/rpitx/pisstv /home/ballon/photo_date.rgb ";
    commande_rpitx << frequence;
    system(commande_rpitx.str().c_str());
}

void Camera::enregistrerPhoto(const std::string annotation){
    
    std::ostringstream commande_still;
    commande_still << "libcamera-still --width 2304 --height 1296 -o /home/ballon/photos/photo_";
    commande_still << std::setw(3) << std::setfill('0') << i << ".jpg ";
    system(commande_still.str().c_str());
    
    std::ostringstream commande_convert;
    commande_convert << "convert -pointsize 80 -box white -draw \"text 10,80 'F4KMN `date +\"%d/%m/%Y %T\"` ";
    commande_convert << annotation << "' \" ";
    commande_convert << "/home/ballon/photos/photo_";
    commande_convert << std::setw(3) << std::setfill('0') << i << ".jpg ";
    commande_convert << "/home/ballon/photos/photo_";
    commande_convert << std::setw(3) << std::setfill('0') << i << ".jpg ";
    system(commande_convert.str().c_str());
    i++;
    
}

