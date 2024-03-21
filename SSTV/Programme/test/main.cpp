/* 
 * File:   main.cpp
 * Author: psimier
 *
 * Created on 21 mars 2024, 08:21
 */

#include <iostream>
#include <sstream>
#include <string>

using namespace std;


int main(int argc, char** argv) {
    
    
    
    const char* nom_variable = "MY_VAR";
    
    setenv(nom_variable, "true", 1);
    // Utilisation de getenv() pour récupérer la valeur de la variable
    const char* valeur_variable = getenv(nom_variable);

    if (valeur_variable != nullptr) {
        std::cout << "La valeur de la variable d'environnement " << nom_variable << " est : " << valeur_variable << std::endl;
    } else {
        std::cout << "La variable d'environnement " << nom_variable << " n'est pas définie." << std::endl;
    }
    
    return 0;
}

