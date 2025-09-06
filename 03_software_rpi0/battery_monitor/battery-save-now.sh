#!/bin/bash
# Script pour déclencher une sauvegarde immédiate de l'état de la batterie

# Récupérer le PID du processus battery_monitor
PID=$(pgrep -x battery_monitor)

if [ -z "$PID" ]; then
    echo "Erreur : le processus battery_monitor n'est pas en cours d'exécution."
    exit 1
fi

# Envoyer le signal SIGHUP pour forcer une sauvegarde
sudo kill -HUP "$PID"
echo "Signal SIGHUP envoyé à battery_monitor (PID=$PID)."
