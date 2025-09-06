// Compilation  make all

#include <csignal>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include "battery.h"

using namespace std;
void signalHandler(int signum);

std::atomic<bool> running(true);
Battery* g_battery = nullptr;   // pointeur global vers l’objet batterie;


int main(){

    try{

        Battery batt(3000);  // capacité en mAh
        g_battery = &batt;

        // charge l’état précédent si dispo
        batt.loadState("/home/ballon/battery_state.txt");

        // enregistre les handlers pour SIGTERM  SIGINT (Ctrl+C) et SIGHUP
        std::signal(SIGTERM, signalHandler);
        std::signal(SIGINT, signalHandler);
        std::signal(SIGHUP,  signalHandler);

        cout <<  " battery_monitor start" << fixed;
        cout << ' '  << setprecision(2) << batt.getTension_V();
        cout << ' '  << setprecision(1) << batt.getCurrent_mA();
        cout << ' '  << setprecision(1) << batt.getCharge_mAh();
        cout << ' '  << setprecision(1) << batt.getSOC() << endl;

        auto lastSave = std::chrono::steady_clock::now();

        while(running){

            batt.update();

            // Sauvegarde périodique toutes les 60 secondes
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - lastSave).count() >= 60) {
                batt.saveState("/home/ballon/battery_state.txt");
                std::cout << "Sauvegarde périodique effectuée." << std::endl;
                lastSave = now;
            }

            this_thread::sleep_for(chrono::seconds(1));
        }

        std::cout << " battery_monitor stop." << std::endl;
        return 0;

    }
    catch (const std::runtime_error &e) {
        cout <<  " Exception caught: " << e.what() << endl;
        return -1;
    }
}



void signalHandler(int signum) {

    if (!g_battery) return;

    if (signum == SIGTERM || signum == SIGINT) {
        std::cout << "Signal reçu (" << signum << "), sauvegarde et arrêt...\r" << std::endl;
        g_battery->saveState("/home/ballon/battery_state.txt");
        running = false;  // permet à la boucle principale de s’arrêter proprement
    }
    else if (signum == SIGHUP) {
        std::cout << "Signal SIGHUP reçu, sauvegarde immédiate.\r" << std::endl;
        g_battery->saveState("/home/ballon/battery_state.txt");
        running = true;  // mais pas d'arrêt → le programme continue
    }
}
