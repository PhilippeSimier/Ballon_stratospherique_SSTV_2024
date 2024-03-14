/* 
 * File:   main.cpp
 * Author: philippe simier
 *
 * Created on 14 mars 2024, 17:13
 */

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>

using namespace std;

// Fonction à exécuter de manière synchronisée

void fonctionSynchronisee() {

    std::cout << "Envoi d'une photo." << std::endl;
    // Prise d'une photo en basse résolution
    system("libcamera-still --width 320 --height 256 -o /home/ballon/photo.jpg");
    // add texte
    system("convert -pointsize 20 -fill black -draw \"text 10,20 'F4KMN `date`'\" /home/ballon/photo.jpg /home/ballon/photo_date.jpg");
    // Convertion en RGB 8 bits
    system("convert -depth 8 /home/ballon/photo_date.jpg /home/ballon/photo_date.rgb");
    // Emission SSTV
    system("sudo /home/ballon/rpitx/pisstv /home/ballon/photo_date.rgb 28680000");
}

int main(int argc, char** argv) {

    while (true) {

        // Obtenir l'heure actuelle
        auto maintenant = chrono::system_clock::now();
        auto tempsActuel = chrono::system_clock::to_time_t(maintenant);
        auto tmMaintenant = *localtime(&tempsActuel);

        // Vérifier si c'est la seconde 0 et que c'est un multiple de 5 minutes
        if (tmMaintenant.tm_sec == 56 && tmMaintenant.tm_min % 5 == 0) {
            // Appeler la fonction synchronisée
            fonctionSynchronisee();

            // Attendre jusqu'à la prochaine minute
            this_thread::sleep_for(chrono::seconds(60));
        } else {
            // Attendre une seconde avant de vérifier à nouveau
            this_thread::sleep_for(chrono::seconds(1));
        }
        
    }

    return 0;
}

