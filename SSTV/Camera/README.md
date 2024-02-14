#  libcamera 
## Description de la commande libcamera

**Libcamera** est une bibliothèque logicielle libre et open-source qui permet d'accéder et de contrôler les caméras sur les systèmes embarqués fonctionnant sous Linux. Elle offre une API C++ aux applications et fonctionne au niveau de la configuration de la caméra et de la récupération des images.

**Utilisations :**

-   **Prise de photos et de vidéos:**  Libcamera peut être utilisée pour prendre des photos et des vidéos avec la caméra connectée au Raspberry Pi 0.

**Commandes libcamera :**

-   **libcamera-still:** Prendre une photo
```bash
libcamera-still --width 320 --height 256 -o photo.jpg
```
-   **libcamera-video:**  Enregistrer une vidéo.
-   **libcamera-hello:**  Afficher des informations sur la caméra.
```bash
ballon@ballon:~ $ libcamera-hello --list-camera
Available cameras
-----------------
0 : imx708_noir [4608x2592] (/base/soc/i2c0mux/i2c@1/imx708@1a)
    Modes: 'SRGGB10_CSI2P' : 1536x864 [120.13 fps - (768, 432)/3072x1728 crop]
                             2304x1296 [56.03 fps - (0, 0)/4608x2592 crop]
                             4608x2592 [14.35 fps - (0, 0)/4608x2592 crop]


```
-   **libcamera-raw:**  Capturer des images brutes.
-   **libcamera-controls:**  Contrôler les paramètres de la caméra.

**Pour plus d'informations :**

-   Site officiel de Libcamera:  [https://libcamera.org/](https://libcamera.org/)
# Incruster du texte
la commande libcamera-still ne possède pas d'option native pour incruster du texte sur une photo.  Cependant, il existe une solution pour contourner ce problème :

Utiliser l'outil `convert` d'ImageMagick pour ajouter du texte à une photo après sa capture.

## Installation
```bash
apt install imagemagick
```
## Vérification de l'installation
```bash
convert --version
Version: ImageMagick 6.9.11-60 Q16 arm 2021-01-25 https://imagemagick.org
```
## Incruster du texte 

l'option [draw](https://imagemagick.org/script/command-line-options.php#draw) 
```bash
convert -pointsize 20 -draw "text 10,20 'F4KMN'" photo.jpg photo_texte.jpg
```


# Changelog

**08/02/2024 : ** Creation du README.md 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur  **Lycée Touchard Le Mans**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


