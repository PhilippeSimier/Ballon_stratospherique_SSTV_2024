#  rpi LoRa files

## Description de rpi LoRa files

**lora_file** est un programme pour émettre et recevoir des messages en modulation LoRa dans un processus indépendant.  

Ce programme utilise deux fils de messages ou MSQ (_Messages Queues_) : 
l'une pour recevoir les messages à émettre en modulation LoRa ;
les message reçus sur la voie radio LoRa  sont écrit dans l'autre fil.

Il n'est pas possible d'émettre et de recevoir simultanément des message sur le canal radio LoRa.
La file **fileTX** contient les messages à émettre tandis que la file **fileRX**  contient les messages reçus. 

Le Modem est configuré en accord avec les informations enregistrées dans le fichier `config.ini`
Ce fichier permet de paramétrer :

 - la **fréquence centrale**  dans la bande 70cm 430 440Mhz des
   radio-amateurs de 433,050 MHz à 434,790 MHz. 
 - la **largeur de bande BW**   réglé par défaut à BW125
 - le **facteur d'étalement SF** (ou nombre de bits par symbole) réglé par défaut à SF12
 - le **Taux de codage CR** réglé à CR5
 
 

# Changelog

**08/02/2024 : ** Creation du README.md 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur  **Lycée Touchard Le Mans**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


