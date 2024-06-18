#include "GestionFile.h"
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>


int main(int argc, char **argv)
{
    try
    {
        GestionFile fileRX;           // Objet pour la gestion des files de messages
        GestionFile fileTX;

        fileRX.obtenirFileIPC(5678);  // Obtenir la file pour la réception key 5678
        fileTX.obtenirFileIPC(5679);  // Obtenir la file pour la réception key 5679

        std::string recu;


        while (true){

            fileRX.lireDansLaFileIPC(recu);
            std::cout << recu << std::endl;
            if (recu == "ping"){
               std::cout << "procedure ping" << std::endl;
               fileTX.ecrireDansLaFileIPC(":F4KMN-8  :reception d'un ping{01");
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
