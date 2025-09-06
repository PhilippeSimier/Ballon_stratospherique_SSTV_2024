# Programme battery_monitor

Le programme **battery_monitor** estime en continu l’état de charge (**SOC**) d’une batterie.  Deux méthodes complémentaires sont utilisées, puis combinées.

## 1. Coulomb counting (intégration du courant)

- Intégration du **courant dans le temps** pour calculer l’énergie consommée.  
- L’intégration est effectuée par la **méthode des rectangles**, avec un pas de **1 seconde**.  

## 2. Méthode tension/SOC

- Comparaison de la **tension mesurée** avec une **courbe de référence**.  
- Cette courbe donne le **SOC typique** d’une batterie **Li-ion 3S**.  

## 3. Fusion des estimations

- Le **SOC calculé par le courant** sert d’estimation principale.  
- Il est **corrigé périodiquement** en fonction du SOC issu de la tension.  

---

## Remarque pratique

La **courbe tension/SOC dépend fortement de la charge appliquée** :  
- **À vide** → la tension reflète correctement l’état de charge.  
- **Sous charge** → la résistance interne provoque une **chute de tension** → la batterie paraît plus déchargée qu’elle ne l’est.  

C’est pourquoi l’algorithme conserve le **coulomb counting** comme méthode principale, et utilise la tension uniquement dans les cas fiables :  

- **Proche de la pleine charge ( U > 12,3 V)**  
- **Proche de la décharge (U < 9,7 V)**  

Dans ces zones, la tension est un indicateur robuste permettant de **corriger automatiquement** l’état de charge calculé.  

## 1. Initialisation

**Objectif :** permettre la reprise de l’intégration du courant après un redémarrage, sans réinitialiser la charge à zéro.  

- Lorsqu’un `SIGTERM` est reçu (envoyé par `systemd` lors de l’extinction), la fonction `saveState()` est appelée pour **sauvegarder la valeur courante de `charge_mAh`** dans un fichier texte.  
- Au redémarrage, le fichier est relu et la valeur sauvegardée est restaurée dans l’attribut `charge_mAh`.  

Ainsi, **battery_monitor** démarre automatiquement au boot, fonctionne en continu et assure une sauvegarde propre avant extinction.  

---

## 2. Installation

1. Copier le fichier `battery-monitor.service` dans le répertoire :
    `/etc/systemd/system/`
 2. Recharger systemd et activer le service
```bash
sudo systemctl daemon-reload
sudo systemctl enable battery-monitor.service
sudo systemctl start battery-monitor.service
```
 3. Vérifier que le service est actif
```bash
sudo systemctl status battery-monitor.service
```
 4. Forcer une sauvegarde immédiate (sans arrêter le service) en envoyant un signal `SIGHUP` :
 ```bash
 sudo kill -HUP <pid>
```
Exemple de sortie système
```bash
ballon@ballon:~/telemetrie $ sudo systemctl status battery-monitor.service
● battery-monitor.service - Battery Monitor
     Loaded: loaded (/etc/systemd/system/battery-monitor.service; enabled; preset: enabled)
     Active: active (running) since Thu 2025-09-04 15:35:58 CEST; 1h 31min ago
   Main PID: 449 (battery_monitor)
      Tasks: 1 (limit: 178)
        CPU: 299ms
     CGroup: /system.slice/battery-monitor.service
             └─449 /home/ballon/telemetrie/battery_monitor

Sep 04 15:35:58 ballon systemd[1]: Started battery-monitor.service - Battery Monitor.
Sep 04 15:35:59 ballon battery_monitor[449]: 04/09/2025 15:35:59 battery_monitor start 12.15 -0.2 2817.1 92.5
Sep 04 15:47:22 ballon battery_monitor[449]: Signal SIGHUP reçu, sauvegarde immédiate.
```
La dernière ligne confirme que le **signal SIGHUP** est bien pris en compte et que la sauvegarde a été effectuée.

## 3. Utilisation

### a) Vérification de la charge sauvegardée
Le programme enregistre régulièrement la valeur de `charge_mAh` dans un fichier texte.  
Pour consulter la dernière valeur sauvegardée :  
```bash
cat /home/ballon/battery_state.txt
```
### b) Sauvegarde manuelle

Il est possible de forcer une sauvegarde immédiate de l’état de charge **sans arrêter le service** :

`kill -HUP <pid>` 

-   Le message `Signal SIGHUP reçu, sauvegarde immédiate.` apparaît alors dans les logs.

### c) Consultation des logs

Pour visualiser les messages produits par le service en temps réel :

`journalctl -fu battery-monitor.service` 

### d) Arrêt et redémarrage du service

-   Arrêt propre avec sauvegarde automatique :
    
    `sudo systemctl stop battery-monitor.service` 
    
-   Redémarrage du service :
    
    `sudo systemctl restart battery-monitor.service`
   
# Changelog

**04/09/2025** :  Création du README.md 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur  **Philippe SIMIER  - F4JRE**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#

