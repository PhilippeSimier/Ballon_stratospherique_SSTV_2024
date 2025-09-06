#include "telemetryAprs.h"

TelemetryAPRS::TelemetryAPRS(const std::string& cs):
    callsign(cs),
    frameNumber(0)
{}

// Conversion tension (9V-12.5V) -> valeur 0..255
int TelemetryAPRS::voltageToADC(double voltage) {
    if (voltage < 9.0) voltage = 9.0;
    if (voltage > 12.5) voltage = 12.5;
    double scaled = (voltage - 9.0) * 255.0 / (12.5 - 9.0);
    return static_cast<int>(std::round(scaled));
}

// Conversion % (0-100) -> valeur 0..255
int TelemetryAPRS::percentToADC(double percent) {
    if (percent < 0.0) percent = 0.0;
    if (percent > 100.0) percent = 100.0;
    double scaled = percent * 255.0 / 100.0;
    return static_cast<int>(std::round(scaled));
}

// Génère une trame T#nnn avec tension et % charge
std::string TelemetryAPRS::createFrame(double voltage, double percent) {

    int adcVolt = voltageToADC(voltage);
    int adcPct  = percentToADC(percent);

    std::ostringstream oss;
    oss << "T#"
        << std::setw(3) << std::setfill('0') << (frameNumber % 1000)
        << "," << std::setw(3) << std::setfill('0') << adcVolt
        << "," << std::setw(3) << std::setfill('0') << adcPct
        << ",000,000,000";  // canaux 3 à 5 à zéro

    frameNumber++;
    return oss.str();
}

// Trames de calibration
std::vector<std::string> TelemetryAPRS::getCalibrationFrames() const {

    std::vector<std::string> frames;
    std::string cs9 = formatCallsign();

    frames.push_back(":" + cs9 + ":PARM=VBat,SOC,Ch3,Ch4,Ch5");
    frames.push_back(":" + cs9 + ":UNIT=Volts,%,0,0,0");
    frames.push_back(":" + cs9 + ":EQNS=0,0.0137,9.0,0,0.392,0,0,0,0,0,0,0,0,0,0,0");

    return frames;
}

// Ajuste le callsign à exactement 9 caractères
std::string TelemetryAPRS::formatCallsign() const {

    std::string cs = callsign;
    if (cs.size() < 9) {
        cs.append(9 - cs.size(), ' ');
    } else if (cs.size() > 9) {
        cs = cs.substr(0, 9);
    }
    return cs;
}


