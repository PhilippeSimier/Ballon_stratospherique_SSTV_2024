#include <chrono>
#include <string>
#include <thread>
#include <sstream>
#include "GestionFile.h"
#include "APRSFrame.h"

using namespace std;

std::string get_local_datetime();
void repondre(std::string requete, std::string source);

GestionFile fileRX;
GestionFile fileTX;
MessageRX message;
int idMessage = 1;

int main() {
    try {
        // Initialisation des files IPC

        fileRX.obtenirFileIPC(5678);
        fileTX.obtenirFileIPC(5679);      
        APRSFrame frame;

        while (true) {
            message = fileRX.lireDansLaFileIPC(2);
            std::string raw(message.text);           
            frame.setRaw(raw.substr(3));    // retire les 3 premiers caractères
            cout << get_local_datetime() << " received " << APRSFrame::typeToString(frame.getFrameType()) << '\t' << raw << endl;  // pour log
            if (frame.getFrameType() == APRSFrame::FrameType::Message && frame.getAddressee()=="F4JRE-5") {
                repondre(frame.getMessage(), frame.getSource());
            }


            // Pause de 1000 ms avant de traiter la trame suivante
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Exception attrapée: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}

void repondre(std::string requete, std::string source) {

    bool ok = false;
    // Ajustement de l'indicatif source sur exactement 9 caractères
    if (source.size() < 9)
        source.append(9 - source.size(), ' ');
    else if (source.size() > 9)
        source.resize(9);

    std::ostringstream os;
    os << ":" << source << ":";

    if (requete.find("QTR?") != std::string::npos){
        os << "QTR " << get_local_datetime() << '{' << idMessage;
        ok = true;
    }

    if (requete.find("QRZ?") != std::string::npos){
        os << "QRZ Ballon Touchard Le Mans" << '{' << idMessage;
        ok = true;
    }

    if (requete.find("QSA?") != std::string::npos){
        os << "QSA RSSI = " << message.RSSI << "dBm SNR = " << message.SNR << "dB";
        os << '{' << idMessage;
        ok = true;
    }

    if (ok){
        fileTX.ecrireDansLaFileIPC(os.str());
        std::cout << get_local_datetime() << " send     Message\t" << os.str() << std::endl;
        ++idMessage;
    }


}

std::string get_local_datetime() {
    std::time_t now = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", std::localtime(&now));
    return std::string(buffer);
}

