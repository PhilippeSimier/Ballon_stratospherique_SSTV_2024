# Télémétrie 

## Description

Les trames **APRS** (Automatic Packet Reporting System) pour les données météorologiques permettent de transmettre des informations sur les conditions météorologiques via des communications radio.

**Les données Météo sont** :

-   **Vent**: Vitesse et direction du vent.
-   **Température**: Température actuelle.
-   **Pluviométrie**: Quantité de précipitations.
-   **Pression barométrique**: Pression atmosphérique.
-   **Humidité relative**: Taux d'humidité.

La description de la trame Weather est dans la documentation  APRS page 74/128  
position less la position du ballon n'est pas transmise dans cette trame.

##  Exemple de trame APRS Weather

    _05311416c...s...g...t062h50b10234

**Symbole**: `_`    Représente une station météo.
  
MDHM : month day hour minutes  ici  
05 Mai  
31 jour dans le mois  
14 heure  
16 minutes  

`c...s...g...`   pas d'information pour le vent (direction, speed, gust) les valeurs sont remplacées par des points.
   
`t062` la température est en ° Fahrenheit  062 = 16,7 °C

`h05` -   : Humidité relative (05%).

`b10234` Pression barométrique (1023.4 hPa). 
  
Les  précipitations sur 24 heures et  depuis minuit sont facultatives (lettre p et P).



# Changelog

**04/09/2022 : ** Creation du README.md 

**08/01/2024 : ** Actualisation de la présentation

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur **Philippe SIMIER** Lycée Touchard Le Mans
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)

<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


