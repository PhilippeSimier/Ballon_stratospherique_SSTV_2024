#include "GestionFile.h"
#include <iostream>

/**
 *
 * @brief GestionFile::GestionFile
 * @param key Une clé (de type int) qui identifie de manière unique la file de messages.
 *
 */
GestionFile::GestionFile(const int key)
{
     if ((fileId = msgget((key_t)key, MSG_FLAG)) == -1)
    {
        throw std::runtime_error("Erreur lors de l'ouverture de la file de messages.");
    }
}

GestionFile::~GestionFile()
{
}




/**
 * @brief GestionFile::ecrireDansLaFileIPC
 * @param payload le message à placer dans la fileTX
 * @return true si le message a pu être placé dans la fileTX
 * @brief std::lock_guard verrouille le mutex mutexTx dès sa construction.
 *        Cela signifie que dès que lock est créé, mutexTx est verrouillé.
 *        Lorsque l'objet lock sort de sa portée son destructeur déverrouille le mutexTx.
 *        même si une exception est levée
 */
bool GestionFile::write(const std::string &payload)
{
    std::lock_guard<std::mutex> lock(mutex);  // création d'un mutex verrouillé immédiatement

    Message message;
    message.type = 2;

    std::strcpy(message.text, payload.c_str());

    if (msgsnd(fileId, &message, sizeof(message.text), 0) == -1)
    {
        throw std::runtime_error("Erreur ecriture fileTX !");
        return false;
    }
    return true;
}

bool GestionFile::write(char* payload, int rssi, float snr)
{
    std::lock_guard<std::mutex> lock(mutex);  // création d'un mutex verrouillé immédiatement
    Message message;
    message.type = 2;
    std::strcpy(message.text, payload);
    message.RSSI = rssi;
    message.SNR = snr;
    
    if (msgsnd(fileId, (void*) &message, sizeof(message)-4, IPC_NOWAIT) == -1)
    {
        throw std::runtime_error("Erreur ecriture fileRX !");
        return false;
    }
    return true;
    
}


/**
 * @brief GestionFile::lireDansLaFile fonction bloquante
 * @param type le type du message
 * @return une structure Message
 */
 
 
Message GestionFile::read(int type){
    
    Message message;
    int ret;

    ret = msgrcv(fileId, &message, sizeof(message) - 4, type, 0);
    if (ret == -1){
        throw std::runtime_error("Erreur lecture fileTX !");
    }   
    return message;
    
}
