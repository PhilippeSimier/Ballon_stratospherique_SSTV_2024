#  MPU6050

## Description 
Le MPU6050 est un capteur inertiel qui combine un accéléromètre et un gyroscope en un seul module.
### Caractéristiques principales

1.  **Accéléromètre à 3 axes :**
    
    -   Mesure l'accélération linéaire dans les trois dimensions (X, Y, Z).
    -   Plages de mesure sélectionnables : ±2g, ±4g, ±8g, ±16g.
3.  **Gyroscope à 3 axes :**
    
    -   Mesure la vitesse angulaire autour des trois axes.
    -   Plages de mesure sélectionnables : ±250, ±500, ±1000, ±2000 degrés par seconde (dps).
4.  **Capteur de température intégré :**
    
    -   Permet des mesures de température internes pour ajuster les calculs des autres capteurs.
5.  **Interface de communication :**
    
    -   Utilise une interface I²C pour la communication avec les microcontrôleurs.
    -   Adresse I²C par défaut : **0x68** (peut être changée à 0x69).
6.  **Conversion analogique-numérique :**
    
    -   Chaque axe du gyroscope et de l'accéléromètre a son propre convertisseur analogique-numérique (ADC) 16 bits, offrant une grande précision.
    - 
## Spécifications techniques

-   **Tension d'alimentation :** 2.375V à 3.46V (typique : 3.3V).
-   **Consommation de courant :** Typiquement 3.9 mA en mode actif.

## Calibration 

 Les registres d'offset du MPU6050 jouent un rôle crucial dans la calibration et l'ajustement des mesures brutes fournies par le capteur. Voici les principales fonctions des registres d'offset du MPU6050 :

### 1. **Correction des erreurs de biais initiales**

Les registres d'offset permettent de corriger les biais initiaux des mesures d'accélération et de rotation. Ces biais peuvent être dus à des imperfections de fabrication, des variations de température, ou des changements environnementaux. En ajustant les valeurs d'offset, on peut obtenir des lectures plus précises et fiables.

### 2. **Calibration du capteur**

Lors de la calibration, les valeurs de sortie du capteur sont comparées à des valeurs de référence connues. Les différences observées sont utilisées pour calculer les valeurs d'offset qui sont ensuite stockées dans les registres correspondants. Cette étape est essentielle pour assurer que le capteur fournit des mesures précises et cohérentes.

Pour définir les biais de l'accéléromètre dans les registres matériels appropriés, il est nécessaire de prendre en compte les valeurs de trim d'usine présentes dans ces registres, qui doivent être ajoutées aux biais calculés de l'accéléromètre. Au démarrage, ces registres contiennent des valeurs non nulles. Par ailleurs, le bit 0 de l'octet de poids faible doit être conservé, car il est utilisé pour les calculs de compensation de température. Les registres de biais de l'accéléromètre attendent une entrée de biais de 2 048 LSB par g, ce qui implique que les biais de l'accéléromètre calculés précédemment doivent être divisés par **8**.
 
 

# Changelog

**08/02/2024 : ** Creation du README.md 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur  **Lycée Touchard Le Mans**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


