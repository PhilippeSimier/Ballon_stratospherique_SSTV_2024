#  libcamera 
## Description de la commande libcamera

**Libcamera** est une bibliothèque logicielle libre et open-source qui permet d'accéder et de contrôler les caméras sur les systèmes embarqués fonctionnant sous Linux. Elle offre une API C++ aux applications qui permet la configuration de la caméra et de la récupération des images.

## Installation
```bash
apt install imagemagick
```


**Utilisations :**

-   **Prise de photos et de vidéos:**  Libcamera peut être utilisée pour prendre des photos et des vidéos avec la caméra connectée au Raspberry Pi 0.

**Commandes libcamera :**
- **libcamera-hello:**   Afficher des informations sur la caméra.
```bash
ballon@ballon:~ $ libcamera-hello --list-camera
Available cameras
-----------------
0 : imx708_noir [4608x2592] (/base/soc/i2c0mux/i2c@1/imx708@1a)
    Modes: 'SRGGB10_CSI2P' : 1536x864 [120.13 fps - (768, 432)/3072x1728 crop]
                             2304x1296 [56.03 fps - (0, 0)/4608x2592 crop]
                             4608x2592 [14.35 fps - (0, 0)/4608x2592 crop]


```


-   **libcamera-still:** Prendre une photo
```bash
libcamera-still --width 320 --height 256 -o photo.jpg
```
-   **libcamera-video:**  Enregistrer une vidéo.
```bash
libcamera-vid -t 10000 -o video.h264 --width 1920 --height 1080 --framerate 30 --inline
```
 `-t <durée>` : Spécifie la durée de l'enregistrement en millisecondes.
 `-o <fichier>` : Spécifie le nom du fichier de sortie.
 `--width <largeur>` et `--height <hauteur>` : Spécifie la résolution de la vidéo.
 `--framerate <taux>` : Spécifie le taux de trame (frames per second) de la vidéo.
 `--inline` : Pour générer un fichier H.264 compatible avec les lecteurs vidéo standard.
- **convertir la vidéo avec ffmpeg**
```bash
ffmpeg -i video.h264 -c:v copy video.mp4
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

Avec l'option [draw](https://imagemagick.org/script/command-line-options.php#draw) 
```bash
convert -pointsize 20 -draw "text 10,20 'F4KMN'" photo.jpg photo_texte.jpg
```
Avec l'option annotate
```bash
convert -pointsize 30 -fill red -annotate +20+30 'F4KMN' photo.jpg photo_texte2.jpg
```
1.  **`convert`**: C'est la commande principale de l'outil ImageMagick, utilisée pour la conversion et la manipulation d'images.
    
2.  **`-pointsize 30`**: Cela définit la taille du texte à 30 points. C'est la taille du texte qui sera ajouté à l'image.
    
3.  **`-fill red`**: Cela spécifie la couleur de remplissage du texte, dans ce cas, le texte sera en rouge.
    
4.  **`-annotate +20+30 'F4KMN'`**: Cette partie de la commande ajoute une annotation textuelle à l'image.
    
    -   `+20+30` spécifie la position relative où le texte sera ajouté. Dans ce cas, le texte sera décalé de 20 pixels à droite et 30 pixels vers le bas à partir du coin supérieur gauche de l'image.
    -   `'F4KMN'` est le texte lui-même, qui sera ajouté à l'image.
5.  **`photo.jpg`**: C'est le nom du fichier source, l'image à laquelle le texte sera ajouté.
    
6.  **`photo_texte2.jpg`**: C'est le nom du fichier de sortie, l'image résultante qui contiendra le texte ajouté.

## Changer la police de caractères

Utilisez l'option **-font FONT NAME** pour sélectionner une police.
La commande suivante affichera la liste complète des polices disponibles :
```bash
convert -list font
```
Pour chaque police, des informations de plusieurs lignes seront affichées, la ligne avec le nom de la police commence par « **Font :** ». C'est ce nom qu'il faut utiliser avec l'option **-font**.


Annotation avec la police Courier-bold
```bash
convert -font Courier-Bold -pointsize 30 -fill red -annotate +20+30 'F4KMN' photo.jpg photo_texte3.jpg
```

## Conversion en rgb

```bash
convert -depth 8 ./photo.jpg ./photo.rgb
```
1.  **`convert`**: C'est la commande principale de l'outil ImageMagick, utilisée pour la conversion et la manipulation d'images.
    
2.  **`-depth 8`**: Cette option spécifie la profondeur de bits par composante pour l'image de sortie. Dans ce cas, elle est fixée à 8 bits, ce qui signifie que chaque composante de couleur (rouge, vert, bleu) aura une plage de valeurs de 0 à 255.
    
3.  **`./photo.jpg`**: C'est le chemin vers le fichier source, dans ce cas, `photo.jpg`. C'est l'image d'entrée qui sera convertie.
    
4.  **`./photo.rgb`**: C'est le chemin vers le fichier de sortie, dans ce cas, `photo.rgb`. La commande convertit l'image d'entrée en un fichier au format brut RGB, qui peut contenir les valeurs de chaque composante de couleur sans aucune compression ou en-tête de fichier spécifique.

# Changelog

**08/02/2024 : ** Creation du README.md 

> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur  **Lycée Touchard Le Mans**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#


