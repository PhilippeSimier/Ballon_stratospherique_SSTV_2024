#include "GestionFile.h"
#include <iostream>

GestionFile::GestionFile()
{
}

GestionFile::~GestionFile()
{
}

/**
 *
 * @brief GestionFile::obtenirFileIPC
 * @param key Une clé (de type int) qui identifie de manière unique la file de messages.
 *
 */
void GestionFile::obtenirFileIPC(const int key)
{
    if ((fileId = msgget((key_t)key, MSG_FLAG)) == -1)
    {
        throw std::runtime_error("Erreur lors de l'ouverture de la file de messages.");
    }
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
bool GestionFile::writeFileTX(const std::string &payload)
{
    std::lock_guard<std::mutex> lock(mutexTx);  // création d'un mutex verrouillé immédiatement

    MessageTX message;
    message.type = 2;

    std::strcpy(message.text, payload.c_str());

    if (msgsnd(fileId, &message, sizeof(message.text), 0) == -1)
    {
        throw std::runtime_error("Erreur ecriture fileTX !");
        return false;
    }
    return true;
}

bool GestionFile::writeFileRX(char* payload, int rssi, float snr)
{
    std::lock_guard<std::mutex> lock(mutexTx);  // création d'un mutex verrouillé immédiatement
    MessageRX message;
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
 * @brief GestionFile::lireDansLaFileRX fonction bloquante
 * @param type le type du message
 * @return une structure MessageRX
 */
 MessageRX GestionFile::readFileRX(int type){

    MessageRX message;
    int ret;

    ret = msgrcv(fileId, &message, sizeof(message) - 4, type, 0);
    if (ret == -1){
        throw std::runtime_error("Erreur lecture fileRX !");
    }   
    return message;
}
 
MessageTX GestionFile::readFileTX(int type){
    
    MessageTX message;
    int ret;

    ret = msgrcv(fileId, &message, sizeof(message) - 4, type, 0);
    if (ret == -1){
        throw std::runtime_error("Erreur lecture fileTX !");
    }   
    return message;
    
}
