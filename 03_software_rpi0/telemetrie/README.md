# Programme telemetrie

Le programme **telemetrie** transmet à intervalle de temps régulier les valeurs de la tension, du taux de charge et du courant de la batterie.

Une trame de télémétrie APRS (T#nnn,...) peux contenir jusqu’à 5 canaux analogiques codés de 000 à 255 (1 octet chacun).

La tension de la batterie doit donc être mise à l’échelle (mapping linéaire).

Plage réelle de la tension  : 9.0 V → 12.5 V
Plage numérique : 0 → 255

`value` est calculé par :  
$ value = \text{round}\!\left( 12.5 - \frac{9.0}{U - 9.0} \times 255 \right) $


   
# Changelog

**08/09/2025** :  Création du README.md 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur  **Philippe SIMIER  - F4JRE**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#

