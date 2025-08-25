# Programme `lora_files`

Le programme **lora_files** assure la gestion de l’émission et de la réception des trames LoRa selon les étapes suivantes :

## 1. Paramétrage de la radio
- La radio est configurée à partir des paramètres définis dans le fichier :  `/home/ballon/configuration.ini`
- Ouverture de deux files de messages :  
- une pour **l’émission**,  
- une pour **la réception**.  
- La radio est placée en **mode réception continue**.

## 2. Boucle d’attente et émission des trames
- Le programme attend passivement les trames présentes dans la file d’émission.  
- Pour chaque trame retirée de la file :  
1. Ajout des en-têtes **LoRa** et **APRS**.  
2. Passage du module radio en **mode émission**.  
3. Transmission de la trame avec ses en-têtes.  
4. Retour du module radio en **mode réception continue**.  

## 3. Réception d’une trame
- À la réception complète d’une trame, le module radio génère une **interruption** qui interrompt la boucle principale.  
- La fonction `callback_Rx` est exécutée :  
- Vérification de l’en-tête **LoRa**.  
- Décapsulation : retrait de l’en-tête LoRa (l’en-tête **APRS** est conservé).  
- La trame ainsi décapsulée, accompagnée du **RSSI** et du **SNR**, est insérée dans la file de réception.  
- Le traitement de cette file est délégué au processus indépendant **`reception`**, chargé de traiter et de répondre aux requêtes.

# Changelog

**25/08/2025** :  Création du README.md 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur  **Philippe SIMIER  - F4JRE**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#

