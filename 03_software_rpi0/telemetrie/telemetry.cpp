#include <iostream>
#include <chrono>
#include <thread>
#include "telemetryAprs.h"
#include "SimpleIni.h"
#include "GestionFile.h"

#define CONFIGURATION "/home/ballon/configuration.ini"
using namespace std;


int main() {

    double charge,tension,soc,courant;

    SimpleIni ini;
    ini.Load(CONFIGURATION);
    string indicatif = ini.GetValue("aprs", "indicatif", "F4ABC");
    TelemetryAPRS aprs(indicatif, "/home/ballon/telemetry_state.txt");

    GestionFile fileTX;
    fileTX.obtenirFileIPC(5679);  // Obtenir la file pour l'émission key 5679

    // Calibration
    for (const auto& line : aprs.getCalibrationFrames()) {
        std::cout << line << std::endl;
        fileTX.ecrireDansLaFileIPC(line);
    }

    while(true){
        // Obtenir l'heure actuelle
        auto maintenant = chrono::system_clock::now();
        auto tempsActuel = chrono::system_clock::to_time_t(maintenant);
        auto tmMaintenant = *localtime(&tempsActuel);

        if (tmMaintenant.tm_min % 2 == 0 &&tmMaintenant.tm_sec ==0) {

            std::ifstream file("/home/ballon/battery_state.txt");

            if (file.is_open()) {
                file >> charge >> tension >> soc >> courant;
                file.close();
                fileTX.ecrireDansLaFileIPC(aprs.createFrame(tension, soc, courant));

            } else {
                std::cerr << "Info : pas de fichier battery_state !" << std::endl;
            }

            this_thread::sleep_for(chrono::seconds(1));
        }
    }

    return 0;
}
