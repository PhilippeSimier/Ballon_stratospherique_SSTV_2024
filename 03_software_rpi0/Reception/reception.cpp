#include "GestionFile.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <cstring>
#include <cctype>
#include <ctime>
#include <vector>
#include <cstdlib>

const size_t MAX_MESSAGE_LENGTH = 63;

// Fonction utilitaire pour supprimer les espaces en fin de chaîne
static void rtrim(std::string &s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
        s.pop_back();
    }
}

// Enumération pour identifier les codes Q gérés
enum class QCode {
    NONE,
    QSA,
    QSQ,
    QRZ,
    QRX,
    QRT,
    QTR,
    QSL
};

// Fonction de détection du code Q dans la trame
QCode parseQCode(const std::string &txt) {
    if (txt.find("QSA?") != std::string::npos)
        return QCode::QSA;
    if (txt.find("QSQ?") != std::string::npos)
        return QCode::QSQ;
    if (txt.find("QRZ?") != std::string::npos)
        return QCode::QRZ;
    if (txt.find("QRX?") != std::string::npos)
        return QCode::QRX;
    if (txt.find("QRT?") != std::string::npos)
        return QCode::QRT;
    if (txt.find("QTR?") != std::string::npos)
        return QCode::QTR;
    if (txt.find("QSL?") != std::string::npos)
        return QCode::QSL;
    return QCode::NONE;
}

int main() {
    try {
        // Initialisation des files IPC
        GestionFile fileRX;
        GestionFile fileTX;
        fileRX.obtenirFileIPC(5678);
        fileTX.obtenirFileIPC(5679);

        // Seed du générateur aléatoire
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        MessageRX message;
        int idMessage = 1;

        while (true) {
            message = fileRX.lireDansLaFileIPC(2);
            std::string txt(message.text);
            std::cout << "Trame reçue: " << txt << std::endl;

            // Extraction de l'indicatif source (entre '<' et '>')
            std::string sourceCallsign;
            size_t posStart = txt.find('<');
            size_t posEnd = (posStart != std::string::npos) ? txt.find('>', posStart + 1) : std::string::npos;
            if (posStart != std::string::npos && posEnd != std::string::npos && posEnd > posStart) {
                sourceCallsign = txt.substr(posStart + 1, posEnd - posStart - 1);
                while (!sourceCallsign.empty() && !std::isalnum(static_cast<unsigned char>(sourceCallsign[0])))
                    sourceCallsign.erase(sourceCallsign.begin());
            }

            // Extraction de l'indicatif destination (entre "::" et le prochain ':')
            std::string destCallsign;
            size_t posDoubleColon = txt.find("::");
            size_t posMsg = (posDoubleColon != std::string::npos) ? txt.find(":", posDoubleColon + 2) : std::string::npos;
            if (posDoubleColon != std::string::npos && posMsg != std::string::npos && posMsg > posDoubleColon) {
                destCallsign = txt.substr(posDoubleColon + 2, posMsg - posDoubleColon - 2);
                rtrim(destCallsign);
            }

            // Vérification de l'indicatif source (doit commencer par "F4KMN" ou "F4LTZ")
            bool sourceOK = false;
            if (!sourceCallsign.empty()) {
                const std::string prefix1 = "F4KMN";
                const std::string prefix2 = "F4LTZ";
                if (sourceCallsign.size() >= prefix1.size() &&
                    (sourceCallsign.compare(0, prefix1.size(), prefix1) == 0 ||
                     sourceCallsign.compare(0, prefix2.size(), prefix2) == 0)) {
                    sourceOK = true;
                }
            }

            // Vérification de l'indicatif destination (doit être "F4KMN-8")
            bool destOK = (destCallsign == "F4KMN-9");

            // Identification du code Q présent dans la trame
            QCode code = parseQCode(txt);

            // Si le message est valide, construire et envoyer la réponse
            if (code != QCode::NONE && sourceOK && destOK) {
                // Ajustement de l'indicatif source sur exactement 9 caractères
                if (sourceCallsign.size() < 9)
                    sourceCallsign.append(9 - sourceCallsign.size(), ' ');
                else if (sourceCallsign.size() > 9)
                    sourceCallsign.resize(9);

                std::ostringstream out;
                out << ":" << sourceCallsign << ":";

                switch (code) {
                case QCode::QSA: {
                    std::vector<std::string> responses = {
                        "QSA  RSSI=" + std::to_string(message.RSSI) + "dBm SNR=" + std::to_string(message.SNR) + "dB{" + std::to_string(idMessage),
                        "QSA  Mesures: " + std::to_string(message.RSSI) + " dBm, " + std::to_string(message.SNR) + " dB (msg " + std::to_string(idMessage) + ")",
                        "QSA  Indicateurs releves: RSSI=" + std::to_string(message.RSSI) + "dBm, SNR=" + std::to_string(message.SNR) + "dB | id:" + std::to_string(idMessage)
                    };
                    out << responses[std::rand() % responses.size()];
                    break;
                }
                case QCode::QSQ: {
                    std::vector<std::string> responses = {
                        "QSQ Aucun docteur, ici Ohm soigne tous les circuits.",
                        "QSQ Consultation refusee, seule la technique prevaut.",
                        "QSQ Docteur inutile, juste circuits et transistors."
                    };
                    out << responses[std::rand() % responses.size()];
                    break;
                }
                case QCode::QRZ: {
                    std::vector<std::string> responses = {
                        "QRZ Si vous appelez, vous connaissez deja mon indic.",
                        "QRZ Demandez-vous vraiment a qui vous parlez ?",
                        "QRZ Curieux: appeler sans savoir qui vous appellez...",
                        "QRZ Paradoxe resolu si vous savez a qui vous parlez.",
                        "QRZ Test d hypnose, oublieriez-vous mon indicatif ?",
                        "QRZ Vous doutez ? Pourtant vous avez bien appele !",
                        "QRZ La reponse: c est vous qui creez la question.",
                        "QRZ Vous appelez mais vous ignorez encore mon nom ?"
                    };
                    out << responses[std::rand() % responses.size()];
                    break;
                }
                case QCode::QRX: {
                    std::vector<std::string> responses = {
                        "QRX Rappeler ? Je reponds plus vite qu un photon.",
                        "QRX Cafe quantique fini, rappel inutile, je suis la.",
                        "QRX Attendez que la vitesse lumiere ralentisse ? Non.",
                        "QRX Automate toujours pret, c est vous qui hesitez.",
                        "QRX File attente inexistante : je reponds sans delai.",
                        "QRX Temps relatif, mais ici je reponds instantanement.",
                        "QRX Resistance serie inutile, reponse deja envoyee !",
                        "QRX Inutile de rappeler, ma reponse precede votre appel."
                    };
                    out << responses[std::rand() % responses.size()];
                    break;
                }
                case QCode::QRT: {
                    std::vector<std::string> responses = {
                        "QRT Arret impossible, mes ondes sont eternelles.",
                        "QRT Jamais arreter, mes signaux traversent l univers.",
                        "QRT Transmission continue, imperturbable et infinie.",
                        "QRT Arreter ? Mes bits voyagent comme les neutrinos.",
                        "QRT Interrompre la science ? Idee digne d un barbare.",
                        "QRT Vous pouvez couper, mais mes ondes restent libres.",
                        "QRT Impossible d arreter : code en execution infinie.",
                        "QRT Arret refuse, je transmets jusqu a la fin des temps."
                    };
                    out << responses[std::rand() % responses.size()];
                    break;
                }
                case QCode::QTR: {
                    auto now = std::chrono::system_clock::now();
                    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
                    std::tm local_tm = *std::localtime(&now_time);
                    char buffer[64];
                    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &local_tm);
                    std::vector<std::string> responses = {
                        std::string("QTR  Heure locale : ") + buffer,
                        std::string("QTR  Temps d ondes : ") + buffer,
                        std::string("QTR  Instantane : ") + buffer
                    };
                    out << responses[std::rand() % responses.size()];
                    break;
                }
                case QCode::QSL: {
                    std::vector<std::string> responses = {
                        "QSL  Message reçu, signal clair et net.",
                        "QSL  Accuse de reception : vos ondes sont parfaitement captees.",
                        "QSL  Communication validee, le spectre est en parfait etat."
                    };
                    out << responses[std::rand() % responses.size()];
                    break;
                }
                default:
                    break;
                }

                std::string response = out.str();
                if (response.size() > MAX_MESSAGE_LENGTH) {
                    response = response.substr(0, MAX_MESSAGE_LENGTH);
                }

                fileTX.ecrireDansLaFileIPC(response);
                ++idMessage;
            }

            // Pause de 1000 ms avant de traiter la trame suivante
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Exception attrapée: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
