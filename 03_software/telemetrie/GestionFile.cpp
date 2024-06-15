#include "GestionFile.h"
#include <iostream>

GestionFile::GestionFile()
{
    creerUneFileIPC();
}

GestionFile::~GestionFile()
{
    // msgctl(msgid, IPC_RMID, NULL);
}

int GestionFile::creerUneFileIPC()
{
    if ((msgid = msgget((key_t)FILE_KEY, MSG_FLAG)) == -1)
    {
        throw std::runtime_error("Erreur lors de la création de la file de messages.");
    }

    return msgid;
}


bool GestionFile::ecrireDansLaFileIPC(const std::string &payload)
{
    std::lock_guard<std::mutex> lock(mutex);

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
