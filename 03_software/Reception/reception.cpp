#include "GestionFile.h"
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>


int main(int argc, char **argv)
{
    try
    {
        GestionFile FileRX;           // Objet pour la gestion des files de messages
        FileRX.obtenirFileIPC(5678);  // Obtenir la file pour la réception key 5678
        std::string recu;

        FileRX.ecrireDansLaFileIPC("test");
        while (true){

            if (FileRX.lireDansLaFileIPC(recu)){
                std::cout << "un message reçu" << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Attendre 1000 ms

        }



    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception attrapée: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
