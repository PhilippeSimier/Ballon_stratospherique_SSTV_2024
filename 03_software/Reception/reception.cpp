#include "GestionFile.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>


int main(int argc, char **argv)
{
    try
    {
        GestionFile fileRX;           // Objet pour la gestion des files de messages
        GestionFile fileTX;
        int message_id = 0;           // A message number
        std::string packet_recu;


        fileRX.obtenirFileIPC(5678);  // Obtenir la file pour la réception LoRa (key 5678)
        fileTX.obtenirFileIPC(5679);  // Obtenir la file pour la émission LoRa  (key 5679)


        while (true){

            fileRX.lireDansLaFileIPC(packet_recu);
            std::cout << packet_recu << std::endl;

            if (packet_recu == "ping"){
                std::ostringstream packet_reponse;
                message_id++;
                packet_reponse << ":F4KMN-8  :reception d'un ping{" << message_id;

                fileTX.ecrireDansLaFileIPC(packet_reponse.str());
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
