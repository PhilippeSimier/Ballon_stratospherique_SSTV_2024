/* 
 * File:   Camera.cpp
 * Author: Philippe SIMIER (F4JRE) Lycée Touchard le Mans
 * 
 * Created on 21 mars 2024, 09:10
 */

#include "Camera.h"

Camera::Camera(const unsigned long _frequence, const std::string _indicatif):
    frequence(_frequence),
    indicatif(_indicatif),
    i(0)
{
}

Camera::Camera(const Camera& orig) {
}

Camera::~Camera() {
}

void Camera::envoyerPhoto(){
    
    // Prise d'une photo en basse résolution
    system("libcamera-still --width 320 --height 256 -o /home/ballon/photo.jpg");

    // add texte & date
    std::ostringstream commande_convert;
    commande_convert << "convert -pointsize 20 -box white -draw \"text 10,20 '" << indicatif;
    commande_convert << "  `date +\"%d/%m/%Y %T\"`'\" /home/ballon/photo.jpg /home/ballon/photo_date.jpg";
    system(commande_convert.str().c_str());

    // Convertion en RGB 8 bits
    system(R"(convert -depth 8 /home/ballon/photo_date.jpg /home/ballon/photo_date.rgb)");

    // Emission SSTV
    std::ostringstream commande_rpitx;
    commande_rpitx << "/home/ballon/rpitx/pisstv /home/ballon/photo_date.rgb ";
    commande_rpitx << frequence;
    system(commande_rpitx.str().c_str());
}

void Camera::enregistrerPhoto(){
    
    std::ostringstream commande_still;
    commande_still << "libcamera-still --width 2304 --height 1296 -o /home/ballon/photos/photo_";
    commande_still << std::setw(3) << std::setfill('0') << i << ".jpg ";
    system(commande_still.str().c_str());
    
    std::ostringstream commande_convert;
    commande_convert << "convert -pointsize 80 -box white -draw \"text 10,80 '"<< indicatif << "  `date +\"%d/%m/%Y %T\"` ";
    commande_convert << "' \" ";
    commande_convert << "/home/ballon/photos/photo_";
    commande_convert << std::setw(3) << std::setfill('0') << i << ".jpg ";
    commande_convert << "/home/ballon/photos/photo_";
    commande_convert << std::setw(3) << std::setfill('0') << i << ".jpg ";
    system(commande_convert.str().c_str());
    i++;
    
}

