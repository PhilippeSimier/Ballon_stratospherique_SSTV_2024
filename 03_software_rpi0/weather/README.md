# Weather 

La télémesure permet de transmettre des données de météorologie, notamment la température, la pression, l’humidité et l’accélération ressentie au niveau de la nacelle du ballon. Cette transmission facilite le suivi et la surveillance en temps réel du ballon pendant sa mission. Les capteurs utilisés dans ce cadre,  le BME280, le LM75 et le MPU6050, sont essentiels pour collecter des données environnementales et de mouvement.

## Description des trames

Les trames **APRS** (Automatic Packet Reporting System) pour les données météorologiques permettent de transmettre des informations sur les conditions météorologiques via des communications radio.

**Les données Météo sont** :

-   **Vent**: Vitesse et direction du vent.
-   **Température**: Température actuelle.
-   **Pluviométrie**: Quantité de précipitations.
-   **Pression barométrique**: Pression atmosphérique.
-   **Humidité relative**: Taux d'humidité.

La description de la trame Weather est dans la documentation  APRS page 74/128
position less la position du ballon n'est pas transmise dans cette trame.

## Localisation d'une station météo

  Afin d'obtenir un symbole sur la carte aprs.fi il est nécessaire d'envoyer une balise (trame position).
```
!4800.48N/00011.98E_/A=000268 
```

  le ! au début indique une position APRS sans timestamp.
  le _ est le symbole aprs pour désigner une station météo

##  La trame APRS télémétrie Weather

Les trames aprs weather sont transmises périodiquement un fois toutes les deux minutes.
exemple de trame
```    
_05311416c...s...g...t062h50b10234 0.00 0.00 1.00
```

**Symbole**: `_`    Représente une station météo.
  
MDHM : month day hour minutes  ici  
05 Mai  
31 jour dans le mois  
14 heure  
16 minutes  

`c...s...g...`   si pas d'information pour le vent (direction, speed, gust) les valeurs sont remplacées par des trois points.
   
`t062` la température est en ° Fahrenheit  062 = 16,7 °C

`h05` -   : Humidité relative (05%).

`b10234` Pression barométrique (1023.4 hPa). 

  
Les  précipitations sur 24 heures et  depuis minuit sont facultatives (lettre p et P).
un commentaire peut être ajouté pour donner des informations complémentaires. nous l'utilisons pour transmettre les composantes de l'accélération.

`0.00 0.00 1.00` l'accélération (g) suivant les axes x,y,z du capteur


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


