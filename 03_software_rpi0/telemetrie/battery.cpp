#include "battery.h"

Battery::Battery(double capacity_mAh):
    capacity_mAh(capacity_mAh),
    charge_mAh(capacity_mAh)
{
    capteur.fixerCalibration_16V();
    lastUpdate = std::chrono::steady_clock::now();
}

void Battery::update()
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - lastUpdate;
    lastUpdate = now;

    double current_mA = capteur.obtenirCourant_mA();  // courant en mA
    // Convention : courant positif = charge, négatif = décharge
    double delta_mAh = current_mA * (elapsed.count() / 3600.0);
    charge_mAh += delta_mAh;

    if (charge_mAh < 0) charge_mAh = 0;
    if (charge_mAh > capacity_mAh) charge_mAh = capacity_mAh;
}

double Battery::getCharge_mAh() const {
    return charge_mAh;
}

// State Of Charge en %
double Battery::getSOC(){

    double soc_coulomb = (charge_mAh / capacity_mAh) * 100.0;
    double voltage = capteur.obtenirTension_V();
    double soc_voltage = socFromVoltage(voltage);

    // Fusion simple : pondération 70% coulomb counting, 30% tension
    return 0.7 * soc_coulomb + 0.3 * soc_voltage;
}

double Battery::getTension_V() {
    return capteur.obtenirTension_V();
}

double Battery::getCurrent_mA() {
    return capteur.obtenirCourant_mA();
}

double Battery::socFromVoltage(double voltage){

    if (voltage >= socCurve.front().voltage) return 100.0;
    if (voltage <= socCurve.back().voltage) return 0.0;

    for (size_t i = 0; i < socCurve.size() - 1; i++) {
        if (voltage <= socCurve[i].voltage && voltage >= socCurve[i+1].voltage) {
            double v1 = socCurve[i].voltage, s1 = socCurve[i].soc;
            double v2 = socCurve[i+1].voltage, s2 = socCurve[i+1].soc;
            return s1 + (s2 - s1) * (voltage - v1) / (v2 - v1);
        }
    }
    return 0.0;
}

// Sauvegarder la charge restante dans un fichier
void Battery::saveState(const std::string &filename){

    std::ofstream file(filename);
    if (file.is_open()) {

        file << charge_mAh;
        file.close();

    } else {
        throw std::runtime_error("Erreur : impossible d'ouvrir le fichier en écriture");

    }
}

// Charger la charge restante depuis un fichier
void Battery::loadState(const std::string &filename){

    std::ifstream file(filename);
    if (file.is_open()) {

        double savedCharge;
        file >> savedCharge;
        if (savedCharge >= 0 && savedCharge <= capacity_mAh) {
            charge_mAh = savedCharge;
        }
        file.close();
    } else {
        std::cerr << "Info : pas de fichier " << filename << ", démarrage avec batterie pleine." << std::endl;
    }

}

