// Compilation  make all

#include <csignal>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include "battery.h"

using namespace std;
std::string get_local_datetime();
void signalHandler(int signum);

std::atomic<bool> running(true);
Battery* g_battery = nullptr;   // pointeur global vers l’objet batterie;


int main(){

    try{

        Battery batt(3000);  // capacité en mAh
        g_battery = &batt;

        // charge l’état précédent si dispo
        batt.loadState("/home/ballon/battery_state.txt");

        // enregistre les handlers pour SIGTERM et SIGINT (Ctrl+C)
        std::signal(SIGTERM, signalHandler);
        std::signal(SIGINT, signalHandler);
        std::signal(SIGHUP,  signalHandler);

        cout << get_local_datetime() << " battery_monitor start" << fixed;
        cout << ' '  << setprecision(2) << batt.getTension_V();
        cout << ' '  << setprecision(1) << batt.getCurrent_mA();
        cout << ' '  << setprecision(1) << batt.getCharge_mAh();
        cout << ' '  << setprecision(1) << batt.getSOC() << endl;

        while(running){

            batt.update();
            this_thread::sleep_for(chrono::seconds(1));
        }

        std::cout << get_local_datetime() << " battery_monitor stop." << std::endl;
        return 0;

    }
    catch (const std::runtime_error &e) {
        cout <<  " Exception caught: " << e.what() << endl;
    }
}

string get_local_datetime() {
    time_t now = time(nullptr);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", localtime(&now));
    return string(buffer);
}

void signalHandler(int signum) {

    if (!g_battery) return;

    if (signum == SIGTERM || signum == SIGINT) {
        std::cout << "Signal reçu (" << signum << "), sauvegarde et arrêt..." << std::endl;
        g_battery->saveState("/home/ballon/battery_state.txt");
        running = false;  // permet à la boucle principale de s’arrêter proprement
    }
    else if (signum == SIGHUP) {
        std::cout << "Signal SIGHUP reçu, sauvegarde immédiate." << std::endl;
        g_battery->saveState("/home/ballon/battery_state.txt");
        running = true;  // mais pas d'arrêt → le programme continue
    }
}
