# Programme telemetrie

Le programme **telemetrie** transmet à intervalle de temps régulier les valeurs de la tension, du taux de charge et du courant de la batterie.

Une trame de télémétrie APRS (T#nnn,...) peux contenir jusqu’à 5 canaux analogiques codés de 000 à 255 (1 octet chacun).
exemple :
```
T#134,144,143,127
```
La tension de la batterie doit donc être mise à l’échelle (mapping linéaire).

 - Plage réelle de la tension  : 9.0 V → 12.5 V
 - Plage numérique : 0 → 255

La formule est $` value = \text{round}\!\left( \frac{U - 9.0}{12.5 - 9.0} \times 255 \right) `$.

Pour le taux de charge 
-   Entrée : pourcentage entre 0 % et 100 %.    
-   Sortie APRS : 0–255 (même principe que la tension).    

La formule est $` value = \text{round}\!\left( \frac{SOC}{100} \times 255 \right) `$.


Pour que les logiciels **APRS** (PinPoint, aprs.fi, UI-View, etc.) affichent la **valeur réelle en volts** au lieu de l’ADC 0–255, il faut transmettre en plus au moins une fois des trames spéciales :

-   **PARM** : noms des capteurs    
-   **UNIT** : unités associées
-   **EQNS** : équation de conversion (y = a·x² + b·x + c)

Exemple pour la tension
La formule est $`  U = \frac{3.5}{255} \times x +9`$ 
Donc :
-   a = 0  
-   b = 3.5 / 255 ≈ 0.013725
-   c = 9.0

En APRS, elles sont envoyées sous forme de **messages** (préfixés par `:` avec pour destinataire  le callsign de l'émetteur.
```
:F4JRE-5 :PARM.VBat,SOC,IBat
:F4JRE-5 :UNIT.V,%,mA
:F4JRE-5 :EQNS.0,0.0137,9.0,0,0.392,0,0,3.92,-500
```
-   `PARM` : première grandeur nommée "Vbat"    
-   `UNIT` : son unité est "V"    
-   `EQNS` : pour chaque des 5 canaux → (a,b,c). Ici :    
    -   Canal 1 : 0,0.0137,9.0
    
**Attention** En APRS, les trames de type message comme `:CALLSIGN :PARM=...` doivent avoir **exactement 9 caractères** pour l’adresse du destinataire (padded avec des espaces si plus court, tronqué si plus long).
**Attention** Les trames de calibration APRS doivent utiliser un **point** (`.`) après `PARM`, `UNIT`, `EQNS`

Le compteur `T#nnn` doit **toujours être croissant et ne jamais repartir à zéro**. Toutefois lorsqu'il atteint 999 il peut dans ce cas là revenir à  0.

**aprs.fi** (et les autres serveurs APRS) rejettent les trames de télémétrie qui ont **le même numéro de séquence** (`T#nnn`) que celles déjà reçues pour un indicatif.
Pour être encore plus robuste si le programme crash ou est arrêté brutalement , la sauvegarde du compteur se fait dans un fichier **après chaque trame envoyée** (plutôt qu’à la destruction de l’objet).
ainsi au redémarrage du programme la transmission de la télémétrie reprend là où elle s'était arrêtée.

-   Chaque trame sauvegarde son numéro → plus aucun risque de duplication signalée par aprs.fi.
    
-   Même après un crash ou une coupure brutale, le compteur reste cohérent. 
 
   
# Changelog

**08/09/2025** :  Création du README.md 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur  **Philippe SIMIER  - F4JRE**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#

