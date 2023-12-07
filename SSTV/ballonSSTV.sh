#!/bin/bash
i=0
OPTIONS='-w 320 -h 256 -o photo.jpg'
while :
do
  raspistill -a 4 -a "F4KMN %X"  $OPTIONS
  convert -depth 8 ./photo.jpg ./photo.rgb
  sudo ./rpitx/pisstv ./photo.rgb 14345000
  echo boucle $i
  ((i++))
  sleep 30
done

