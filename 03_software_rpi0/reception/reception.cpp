#include <chrono>
#include <string>
#include <thread>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "SimpleIni.h"
#include "GestionFile.h"
#include "APRSFrame.h"

#define CONFIGURATION "/home/ballon/configuration.ini"
using namespace std;

std::string get_local_datetime();
void repondre(std::string requete, std::string source);
int savePosition(const double latitude, const double longitude, const double altitude);
void touch(const std::string& chemin);
bool isLoRaHeader(const std::string& frame);

GestionFile fileRX;
GestionFile fileTX;
MessageRX message;
SimpleIni ini;

int idMessage = 1;

int main() {
    try {
        // Initialisation des files IPC

        fileRX.obtenirFileIPC(5678);
        fileTX.obtenirFileIPC(5679);
        APRSFrame frame;
        ini.Load(CONFIGURATION);
        string indicatif = ini.GetValue("aprs", "indicatif", "F4ABC");

        while (true) {
            message = fileRX.lireDansLaFileIPC(2);
            string raw(message.text);

            if (isLoRaHeader(raw)) {
                frame.setRaw(raw.substr(3));    // retire les 3 premiers caractères
                cout << get_local_datetime() << " received " << APRSFrame::typeToString(frame.getFrameType()) << '\t' << raw.substr(3) << endl;  // pour log
                if (frame.getFrameType() == APRSFrame::FrameType::Message && frame.getAddressee() == indicatif) {
                    repondre(frame.getMessage(), frame.getSource());
                }
                // si la trame est une position reçue du tracker interne
                if (frame.getFrameType() == APRSFrame::FrameType::Position && frame.getSource() == indicatif) {
                    savePosition(frame.getLatitude(), frame.getLongitude(), frame.getAltitude());
                }
            }

            // Pause de 100 ms avant de traiter la trame suivante
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
    catch (const exception &e) {
        cerr << "Exception attrapée: " << e.what() << endl;
    }

    return EXIT_SUCCESS;
}

void repondre(string requete, string source) {

    bool ok = false;
    // Ajustement de l'indicatif source sur exactement 9 caractères
    if (source.size() < 9)
        source.append(9 - source.size(), ' ');
    else if (source.size() > 9)
        source.resize(9);

    ostringstream os;
    os << ":" << source << ":";

    if (requete.find("QTR?") != string::npos){
        os << "QTR " << get_local_datetime() << '{' << idMessage;
        ok = true;
    }

    if (requete.find("QRZ?") != string::npos){
        os << "QRZ Ballon Touchard Le Mans" << '{' << idMessage;
        ok = true;
    }

    if (requete.find("QSA?") != string::npos){
        os << "QSA RSSI = " << message.RSSI << "dBm SNR = " << message.SNR << "dB";
        os << '{' << idMessage;
        ok = true;
    }

    if (requete.find("MIRE") != string::npos){
        touch("/ramfs/mire");   // création d'un fichier mire dans ramfs
        os << "Mire = ON" << '{' << idMessage;
        ok = true;
    }

    if (requete.find("PHOTO") != string::npos){
        if (remove("/ramfs/mire") == 0) {   // suppression du fichier mire
            os << "Mire = OFF" << '{' << idMessage;
            ok = true;
        }
    }

    if (ok){
        fileTX.ecrireDansLaFileIPC(os.str());
        cout << get_local_datetime() << " send     Message\t" << os.str() << endl;
        ++idMessage;
    }
}

string get_local_datetime() {
    time_t now = time(nullptr);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", localtime(&now));
    return string(buffer);
}

void touch(const string& chemin) {

    if (!filesystem::exists(chemin)) {
        // Crée un fichier vide
        ofstream(chemin).close();
    } else {
        // Met à jour la date de modification
        auto maintenant = filesystem::file_time_type::clock::now();
        filesystem::last_write_time(chemin, maintenant);
    }
}

int savePosition(const double latitude, const double longitude, const double altitude){

    std::ofstream fichier("/ramfs/position.txt"); // ouvre (ou crée) le fichier

    if (!fichier) {
        std::cerr << "Erreur d'ouverture du fichier !" << std::endl;
        return 1;
    }

    // Écrire les trois valeurs séparées par un espace
    fichier << fixed << setprecision(5);
    fichier << latitude << " " << longitude << " " << setprecision(1) << altitude << std::endl;

    fichier.close();
    return 0;
}

bool isLoRaHeader(const std::string& frame) {
    // Vérifier qu'il y a au moins 3 octets
    if (frame.size() < 3) return false;

    return (static_cast<unsigned char>(frame[0]) == 0x3C &&  // '<'
            static_cast<unsigned char>(frame[1]) == 0xFF &&
            static_cast<unsigned char>(frame[2]) == 0x01);
}


