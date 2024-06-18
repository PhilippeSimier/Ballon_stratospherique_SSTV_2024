#include "GestionFile.h"
#include <iostream>

GestionFile::GestionFile()
{

}

GestionFile::~GestionFile()
{
}

/**
 * @brief GestionFile::obtenirFileIPC
 * @param key Une clé (de type int) qui identifie de manière unique la file de messages.
 *
 */
void GestionFile::obtenirFileIPC(const int key)
{
    if ((msgid = msgget((key_t)key, MSG_FLAG)) == -1)
    {
        throw std::runtime_error("Erreur lors de la création de la file de messages.");
    }
}

/**
 * @brief GestionFile::ecrireDansLaFileIPC
 * @param payload le message à placer dans la file
 * @return true si le message a pu être mis dans la file
 * @brief std::lock_guard verrouille le mutex mutexTx dès sa construction.
 *        Cela signifie que dès que lock est créé, mutexTx est verrouillé.
 *        Lorsque l'objet lock sort de sa portée son destructeur déverrouille le mutexTx.
 *        même si une exception est levée
 */
bool GestionFile::ecrireDansLaFileIPC(const std::string &payload)
{
    std::lock_guard<std::mutex> lock(mutexTx);  // création d'un mutex verrouillé immédiatement

    Message message;
    message.mtype = 2;

    std::strcpy(message.mtext, payload.c_str());

    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1)
    {
        throw std::runtime_error("Erreur lors de l'envoi du message.");
        return false;
    }

    return true;
}

bool GestionFile::lireDansLaFileIPC(std::string &recu){

    Message message;
    int ret;
    bool retour = false;

    ret = msgrcv(msgid, (void*) &message, sizeof(message.mtext), 0, IPC_NOWAIT);
    if (ret != -1){
        std::cout << message.mtext << std::endl;
        retour = true;
    }
    return retour;

}
