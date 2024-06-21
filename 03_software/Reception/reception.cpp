#include "GestionFile.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <cstring>   // pour les char*


int main(int argc, char **argv)
{
    try
    {
        GestionFile fileRX;           // Objet pour la gestion des files de messages
        GestionFile fileTX;

        fileRX.obtenirFileIPC(5678);  // Obtenir la file pour la réception key 5678
        fileTX.obtenirFileIPC(5679);  // Obtenir la file pour l'émission   key 5679

        MessageRX message;
        int idMessage = 1;

        while (true){

            message = fileRX.lireDansLaFileIPC(2);
            std::cout << message.text << std::endl;

            if ( std::strcmp(message.text, "QSA?") == 0 ){
               std::cout << "requête QSA?" << std::endl;
               std::ostringstream out;
               out << ":f4kmn    :QSA  RSSI=" << message.RSSI << "dBm SNR=" << message.SNR << "dB{" <<idMessage;
               fileTX.ecrireDansLaFileIPC(out.str());
               idMessage++;
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
