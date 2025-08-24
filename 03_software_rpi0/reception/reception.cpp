#include <chrono>
#include <string>
#include <thread>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "SimpleIni.h"
#include "GestionFile.h"
#include "APRSFrame.h"
#include "Position.h"

#define CONFIGURATION "/home/ballon/configuration.ini"
using namespace std;

std::string get_local_datetime();
void repondre(std::string requete, std::string source);
void touch(const std::string& chemin);


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

        double latitude = ini.GetValue<double>("beacon", "latitude", 48.0);
        double longitude = ini.GetValue<double>("beacon", "longitude", 0.0);
        double altitude = ini.GetValue<double>("beacon", "altitude", 1);
        std::string comment = ini.GetValue<string>("beacon", "comment", "comment default");
        char symbol_table = ini.GetValue<char>("beacon", "symbol_table", '/');
        char symbol  = ini.GetValue<char>("beacon", "symbol", 'O');

        Position posBallon(latitude, longitude, altitude, symbol_table, symbol, comment); // icon ballon
        posBallon.save("/ramfs/position.txt");
        std::string positionAPRS = posBallon.getPduAprs();
        fileTX.ecrireDansLaFileIPC(positionAPRS);
        std::cout << get_local_datetime() << " send     Position\t" << positionAPRS  << std::endl;;


        // envoie un message pour informer que LoRa est prêt
        fileTX.ecrireDansLaFileIPC(":f4kmn    :Ballon Touchard QRV");
        cout << get_local_datetime() << " send     Message\t" << ":f4kmn    :Ballon Touchard QRV" << endl;

        while (true) {

            message = fileRX.lireDansLaFileIPC(2);
            string raw(message.text);
            frame.setRaw(raw);
            cout << get_local_datetime() << " received " << APRSFrame::typeToString(frame.getFrameType()) << '\t' << raw << endl;  // pour log

            // si la trame est un message qui nous est addressé
            if (frame.getFrameType() == APRSFrame::FrameType::Message && frame.getAddressee() == indicatif) {
                repondre(frame.getMessage(), frame.getSource());
            }
            // si la trame est une position reçue du tracker interne
            if (frame.getFrameType() == APRSFrame::FrameType::Position && frame.getSource() == indicatif) {
                posBallon.set(frame.getLatitude(), frame.getLongitude(), frame.getAltitude());
                posBallon.save("/ramfs/position.txt");
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






