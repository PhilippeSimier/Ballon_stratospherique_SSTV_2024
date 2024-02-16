#  rpitx
## Description de rpitx

**rpitx** est un logiciel open-source qui permet à un Raspberry Pi de fonctionner comme un émetteur radio. Le nom "rpitx" est dérivé de "Raspberry Pi" et "transmitter" (émetteur en anglais).

Il permet au Raspberry Pi de générer des signaux RF (Radio Fréquence) sur différentes gammes de fréquences. Cela peut inclure des fréquences radio FM, des signaux de télévision SSTV, des signaux de télécommande, etc.

Branchez un fil sur **GPIO 4**, c'est-à-dire la broche 7 du connecteur d’extension. Ce fil fait office d'antenne. La longueur optimale du fil dépend de la fréquence sur laquelle vous souhaitez transmettre, mais cela peut fonctionne avec quelques centimètres pour des tests locaux.


## Installation rpitx
```bash
apt install git
git clone https://github.com/F5OEO/rpitx
cd rpitx
./install.sh
```
il suffit de cloner le dépôt rpitx présent sur github, puis de lancer le script d’installation. Vous devez accepter la modification du fichier `config.txt`
 
 ```bash
 In order to run properly, rpitx need to modify /boot/config.txt. Are you sure (y/n) y
Set GPU to 250Mhz in order to be stable
gpu_freq=250
force_turbo=1
Installation completed !
root@ballon:/home/ballon/rpitx#
 ```

Il faudra redémarrer le Raspberry Pi avant toute utilisation de rpitx.
```bash
sudo reboot
```
## Test 
Le premier test  est d’émettre une porteuse afin de vérifier avec une clé SDR et le récepteur logiciel `Gqrx` la bonne réception du signal.

Sur la rpi,  lancer le scripte  `./easytest.sh`
Avec l'option F du menu, choisir une fréquence de base pour les tests.  par exemple 28.680 Mhz.
Avec l'option 0 du menu, lancer l'émission en continu d'une porteuse, une fenêtre indiquant **Transmitting** apparaît. 
Appuyer sur Ok pour arrêter l’émission de la porteuse. 

Sur le PC  

 - tester la présence de la clé SDR avec `rtl-test`
```bash
rtl_test
Found 1 device(s):
  0:  Realtek, RTL2838UHIDIR, SN: 00000001

Using device 0: Generic RTL2832U OEM
Detached kernel driver
Found Rafael Micro R828D tuner
```
Ouvrir Gqrx dans la fenêtre device choisir la clé SDR
![le paramétrage de Gqrx](/SSTV/rpitx/Gqrx_water-fall.png ) 

 - Le WaterFall

![le waterfall](/SSTV/rpitx/reglage_device.png ) 

# Slow Scan Television (SSTV)
Il s'agit du mode de transmission d'images utilisant la modulation audio (mode USB Upper Side Band = Bande Latérale Supérieure). Vous avez besoin d'un logiciel supplémentaire pour décoder et afficher l'image. 
Cette démo utilise le mode **Martin1** de sstv.

# Changelog

**08/02/2024 : ** Creation du README.md 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur  **Lycée Touchard Le Mans**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


