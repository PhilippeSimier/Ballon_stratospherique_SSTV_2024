# Accéder aux fichiers distant sshfs

## Description sshfs

La commande `sshfs` sous Linux permet de monter un système de fichiers distant via SSH (Secure Shell). Plus précisément, `sshfs` (SSH Filesystem) utilise le protocole SSH pour accéder aux fichiers d'un système distant comme s'ils faisaient partie du système de fichiers local. Cela permet de manipuler les fichiers distants avec les mêmes commandes que celles utilisées pour les fichiers locaux, offrant une solution pratique pour le transfert sécurisé de fichiers et l'accès à des ressources distantes.


##  installation

    sudo apt install sshfs
    mkdir remote

## Utilisation générale

La syntaxe de base pour utiliser `sshfs` est la suivante :

`sshfs [user@]hostname:[directory] mountpoint [options]` 

-   **`[user@]hostname`** : Spécifie le nom d'utilisateur et le nom d'hôte du serveur SSH distant. Si l'utilisateur est omis, l'utilisateur courant est utilisé.
-   **`[directory]`** : Le répertoire sur le serveur distant que vous voulez monter. Si ce répertoire est omis, le répertoire personnel de l'utilisateur distant est utilisé.
-   **`mountpoint`** : Le point de montage sur le système local où le répertoire distant sera monté.
-   **`[options]`** : Les options supplémentaires pour personnaliser le montage 

## Exemple d'utilisation pour la carte ballon

Pour monter un répertoire distant `/home/ballon` sur la carte à l'adresse `192.168.1.36`  dans le répertoire local `remote` en tant qu'utilisateur `ballon`, vous utiliseriez la commande suivante :

    sshfs ballon@192.168.1.36:/home/ballon remote/

# Changelog

**07/06/2024 : ** Creation du README.md pour la commande sshfs

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur **Philippe SIMIER** Lycée Touchard Le Mans
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)

<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


