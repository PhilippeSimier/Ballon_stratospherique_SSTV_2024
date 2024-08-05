## Carte Ballon stratosphérique 2024

### Installation des outils I2C

    apt install i2c-tools
    i2cdetect -y 1

### Installation de git

    apt install git

### Installation de la bibliothèque wiringPi

    git clone https://github.com/WiringPi/WiringPi.git
    cd WiringPi
    ./build
    
 ### Activer la gestion des files sur le raspberry  
  
**/boot/config.txt** est un fichier de configuration important utilisé pour configurer le matériel et certains paramètres système avant le démarrage du noyau Linux.  

    sudo nano /boot/config.txt

 ajouter à la fin :

     kernel.msgmni=1024

 puis redémarrer

     sudo reboot
Vérification des paramètres actuels

 1.  Vérifiez la limite maximale des messages par file :
```bash
	cat /proc/sys/kernel/msgmax
	8192
```

 2. Vérifiez la limite totale de messages dans le système :
       
```bash
    cat /proc/sys/kernel/msgmni
    32000
 ```      

 3. Vérifiez la taille maximale d'une file de messages :

```bash
    cat /proc/sys/kernel/msgmnb
    16384
```
