# Programme **sstv**

Le programme **sstv** fonctionne en deux grandes étapes :  

## 1. Initialisation de l’image de mire  
- L’image de mire est annotée avec l’indicatif du ballon ainsi que la fréquence d’émission.  
- Elle est ensuite copiée dans le répertoire `/ramfs/`.  

## 2. Boucle de traitement continue  
Le programme entre ensuite dans une boucle infinie :  
- **Si le fichier `stop` est présent** dans `/ramfs/` → le programme reste inactif.  
- **Si le fichier `mire` est présent** dans `/ramfs/` → le programme envoie l’image de la mire en SSTV (mode *Martin 1*), puis se met en pause pendant 60 secondes.  
- **Si aucun fichier `stop` ni `mire` n’est présent** → le programme capture une photo du ballon et l’envoie en SSTV (mode *Martin 1*).  
- De plus, **à la seconde 30 de chaque minute**, une photo en haute résolution est enregistrée dans le répertoire `photos` (sauf si une émission SSTV est déjà en cours).  


   
# Changelog

**25/08/2025** :  Création du README.md 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur  **Philippe SIMIER  - F4JRE**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#

