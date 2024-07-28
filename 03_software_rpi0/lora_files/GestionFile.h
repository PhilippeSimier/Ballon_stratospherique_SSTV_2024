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

// Structure pour le message

struct Payload {
    long type;
    char text[MAX_MESSAGE_SIZE];
    float SNR;
    int RSSI;
};

class GestionFile {
private:
    std::mutex mutex;
    int fileId;

public:
    GestionFile(const int key);
    ~GestionFile();

    
    Payload read(int type);
    
    bool write(const std::string &payload);  
    bool write(char* payload, int rssi, float snr);
    
    
};

#endif // GESTIONFILE_H
