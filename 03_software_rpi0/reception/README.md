# Programme réception
 Le programme **réception** exécute les étapes suivantes :

##  1.  Initialisation des trames APRS au démarrage
    
   

 - Construction d’une première trame APRS de type _position beacon_.
 - Construction d’une seconde trame APRS de type _message_ signalant que
   le ballon est opérationnel.
 - Ces deux trames sont insérées dans la **file d’émission** (queue).
   Leur transmission est déléguée au processus indépendant `lora_file`,
   chargé de la diffusion radio via modulation LoRa.

        
## 2.  Boucle d’attente et de traitement  
Le programme entre ensuite dans une boucle d’attente passive sur les trames entrantes.  
    Pour chaque trame reçue :

 - La trame est **parsée** afin d’extraire ses champs (type,
   destination, payload, etc.).

        
## 3.  Logique de traitement selon le type de trame
    
   **Message → ballon** : si le destinataire est le ballon et que le contenu correspond à une requête connue, une trame de réponse est générée et envoyée.
        
**Position → ballon (traqueur interne)** : la position courante du ballon est mise à jour, puis enregistrée dans un fichier de données (`/ramfs/position.txt`)
        
**Position → station sol** : la distance entre la position reçue (station) et la position courante du ballon est calculée, puis consignée dans les fichiers de logs.


