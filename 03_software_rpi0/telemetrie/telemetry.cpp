#include <iostream>
#include "telemetryAprs.h"

int main() {
    TelemetryAPRS aprs("F4ABC-1");

    // Calibration
    for (const auto& line : aprs.getCalibrationFrames()) {
        std::cout << line << std::endl;
    }

    std::cout << std::endl;

    // Télémétrie (Voltage, Pourcentage)
    std::cout << aprs.createFrame(9.0, 0.0) << std::endl;
    std::cout << aprs.createFrame(10.0, 50.0) << std::endl;
    std::cout << aprs.createFrame(12.5, 100.0) << std::endl;

    return 0;
}
