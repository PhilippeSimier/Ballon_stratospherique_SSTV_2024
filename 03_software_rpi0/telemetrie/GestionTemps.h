/*
 * File:   GestionTemps.h
 * Author: dbrochard
 *
 * Created on 27 mars 2024, 15:01
 */

#ifndef GestionTemps_H
#define GestionTemps_H

#include <iomanip>
#include <ctime>
#include <chrono>
#include <iostream>

// Structure pour stocker le temps
struct Temps
{
    std::string annee{};
    std::string mois{};
    std::string jour{};
    std::string heure{};
    std::string minute{};
    std::string seconde{};
};


class GestionTemps
{

public:
    GestionTemps(); // Constructeur
    ~GestionTemps(); // Destructeur

    std::tm obtenirHeureActuelle(); // Méthode pour obtenir l'heure actuelle
    void majDate();                 // Méthode pour mettre à jour la date
    std::string getDateFormatee();  // Méthode pour obtenir la date et l'heure sous format aaaa-mm-jj hh:mm:ss
    std::string getDateAprs();      // Méthode pour obtenir la date mois jour heure minute format aprs
    std::string generateFilename(); // Méthode pour générer un nom de fichier




private:
    Temps temps; // Variable pour stocker le temps

};

#endif /* GestionTemps_H */
