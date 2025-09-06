#ifndef TELEMETRYAPRS_H
#define TELEMETRYAPRS_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

class TelemetryAPRS
{
public:
    TelemetryAPRS(const std::string& cs);

    static int voltageToADC(double voltage); // Conversion tension (9V-12.5V) -> valeur 0..255
    static int percentToADC(double percent); // Conversion % (0-100) -> valeur 0..255

    std::string createFrame(double voltage, double percent);
    std::vector<std::string> getCalibrationFrames() const;

private:
    std::string callsign;
    int frameNumber;
    std::string formatCallsign() const;
};

#endif // TELEMETRYAPRS_H
