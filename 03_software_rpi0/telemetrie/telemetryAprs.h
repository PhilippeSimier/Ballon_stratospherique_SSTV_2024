#ifndef TELEMETRYAPRS_H
#define TELEMETRYAPRS_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

class TelemetryAPRS
{
public:

    TelemetryAPRS(const std::string& cs,
                  const std::string& stateFilePath = "telemetry_state.txt");

    static int voltageToADC(double voltage); // Conversion tension (9V-12.5V) -> valeur 0..255
    static int percentToADC(double percent); // Conversion % (0-100) -> valeur 0..255
    static int currentToADC(double current_mA); // Conversion courant (-500mA à +500mA) -> valeur 0..255

    std::string createFrame(double voltage, double percent, double current);
    std::vector<std::string> getCalibrationFrames() const;

private:

    std::string callsign;
    int frameNumber;
    std::string stateFile;

    std::string formatCallsign() const;
    void saveFrameNumber() const;
    int  loadFrameNumber() const;
};

#endif // TELEMETRYAPRS_H
