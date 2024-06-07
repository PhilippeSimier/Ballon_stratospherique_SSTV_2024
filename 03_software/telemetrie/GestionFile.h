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

const int MAX_MESSAGE_SIZE = 100;      // Taille maximum du message
const int FILE_KEY = 5679;             // Clé de la file de messages
const int MSG_FLAG = 0666 | IPC_CREAT; // Flag de création de la file de messages

// Structure pour le message
struct Message
{
    long mtype;
    char mtext[MAX_MESSAGE_SIZE];
};

class GestionFile
{
private:
    std::mutex mutex;
    int msgid;

public:
    GestionFile();
    ~GestionFile();
    bool ecrireDansLaFileIPC(const std::string &payload);
    int creerUneFileIPC();
};

#endif // GESTIONFILE_H