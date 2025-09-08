#include "telemetryAprs.h"

TelemetryAPRS::TelemetryAPRS(const std::string& cs, const std::string& stateFilePath):
    callsign(cs),
    frameNumber(0),
    stateFile(stateFilePath)
{
    frameNumber = loadFrameNumber();   // le numéro de séquence continue là où il s’était arrêté
    if ( (frameNumber < 0) || (frameNumber > 999) ) {
        frameNumber = 0; // sécurité
    }
}

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

// Conversion courant (-500mA à +500mA) -> valeur 0..255
int TelemetryAPRS::currentToADC(double current_mA) {
    if (current_mA < -500.0) current_mA = -500.0;
    if (current_mA > 500.0)  current_mA = 500.0;
    double scaled = (current_mA + 500.0) * 255.0 / 1000.0;
    return static_cast<int>(std::round(scaled));
}

// Génère une trame T#nnn avec tension SOC et courant
std::string TelemetryAPRS::createFrame(double voltage, double percent, double current) {

    int adcVolt = voltageToADC(voltage);
    int adcPct  = percentToADC(percent);
    int adcCur  = currentToADC(current);

    std::ostringstream oss;
    oss << "T#"
        << std::setw(3) << std::setfill('0') << frameNumber
        << "," << std::setw(3) << std::setfill('0') << adcVolt
        << "," << std::setw(3) << std::setfill('0') << adcPct
        << "," << std::setw(3) << std::setfill('0') << adcCur;

    frameNumber++;
    if (frameNumber > 999) frameNumber = 0;
    saveFrameNumber();  // Sauvegarde immédiate après chaque trame
    return oss.str();
}

// Trames de calibration
std::vector<std::string> TelemetryAPRS::getCalibrationFrames() const {

    std::vector<std::string> frames;
    std::string cs9 = formatCallsign();

    frames.push_back(":" + cs9 + ":PARM.VBat,SOC,IBat");
    frames.push_back(":" + cs9 + ":UNIT.V,%,mA");
    frames.push_back(":" + cs9 + ":EQNS.0,0.0137,9.0,0,0.392,0,0,3.92,-500");

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

void TelemetryAPRS::saveFrameNumber() const {

    std::ofstream ofs(stateFile);
    if (ofs) {
        ofs << frameNumber;
    }
}

int  TelemetryAPRS::loadFrameNumber() const{

    std::ifstream ifs(stateFile);
    int n = 0;
    if (ifs >> n) return n;
    return 0;

}


