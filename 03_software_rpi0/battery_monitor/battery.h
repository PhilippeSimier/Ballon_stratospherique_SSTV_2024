#ifndef BATTERY_H
#define BATTERY_H

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include "ina219.h"

struct VoltageSOC {
    double voltage;
    double soc;
};

class Battery
{
public:
    Battery(double capacity_mAh);
    void update();
    double getCharge_mAh() const;
    double getSOC();
    double getTension_V();
    double getCurrent_mA();

    void saveState(const std::string &filename);
    void loadState(const std::string &filename);


private:

    double capacity_mAh;        // capacité nominale en mAh
    double charge_mAh;          // charge actuelle estimée en Ah

    std::chrono::steady_clock::time_point lastUpdate;
    ina219 capteur;

    // Interpolation linéaire de la courbe tension/SOC
    double socFromVoltage(double voltage);

    // Courbe tension/SOC pour le pack 3S (12.48V max mesuré)
    std::vector<VoltageSOC> socCurve = {
        {12.48, 100}, {12.18, 90}, {11.88, 80}, {11.58, 70},
        {11.28, 60}, {10.98, 40}, {10.68, 20}, {10.38, 10},
        {10.08, 5}, {9.60, 0}
    };



};

#endif // BATTERY_H
