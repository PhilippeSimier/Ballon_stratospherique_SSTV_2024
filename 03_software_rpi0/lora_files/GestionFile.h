/**
 * GestionFile.h est la classe qui gère les files IPC
 *
 *
 */

#ifndef GESTIONFILE_H
#define GESTIONFILE_H

#include <string>    // Inclut la bibliothèque standard de C++ pour les chaînes de caractères
#include <mutex>     // Inclut la bibliothèque standard de C++ pour les mutex
#include <stdexcept> // Inclut la bibliothèque standard de C++ pour les exceptions
#include <cstring>   // Inclut la bibliothèque standard de C pour strncpy
#include <sys/ipc.h> // Inclut la bibliothèque de système pour IPC_CREAT
#include <sys/msg.h> // Inclut la bibliothèque de système pour les files de messages

const int MAX_MESSAGE_SIZE = 256; // Taille maximum du message
const int MSG_FLAG = 0666 | IPC_CREAT; // Flag de création de la file de messages

// Structure pour le message TX

struct MessageTX {
    long type;
    char text[MAX_MESSAGE_SIZE];
};

// Structure pour le message RX

struct MessageRX {
    long type;
    char text[MAX_MESSAGE_SIZE];
    float SNR;
    int RSSI;
};

class GestionFile {
private:
    std::mutex mutexTx;
    int fileId;

public:
    GestionFile();
    ~GestionFile();

    void obtenirFileIPC(const int key);
    
    bool writeFileTX(const std::string &payload);
    MessageTX readFileTX(int type);
    
    bool writeFileRX(char* payload, int rssi, float snr);
    MessageRX readFileRX(int type);
    
};

#endif // GESTIONFILE_H
