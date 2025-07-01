import re

def supprimer_doublons(coordonnees):
    uniques = []
    vus = set()
    for lon, lat, alt in coordonnees:
        cle = (round(lon, 7), round(lat, 7), round(alt, 1))  # tolérance
        if cle not in vus:
            uniques.append((lon, lat, alt))
            vus.add(cle)
    return uniques

def dms_vers_decimal(dm, direction):
    degres = int(dm[:2])
    minutes = float(dm[2:])
    decimal = degres + minutes / 60
    if direction in ['S', 'W', 'O']:
        decimal = -decimal
    return decimal

def extraire_coordonnees(lignes):
    coords = []
    vus = set()
    for ligne in lignes:
        match = re.search(r'!(\d{4}\.\d{2})([NS])/(\d{5}\.\d{2})([EO])[^A]*A=(\d+)', ligne)
        if match:
            lat_dm, lat_dir, lon_dm, lon_dir, alt_ft = match.groups()
            lat = round(dms_vers_decimal(lat_dm, lat_dir), 7)
            lon = round(dms_vers_decimal(lon_dm, lon_dir), 7)
            alt_m = round(int(alt_ft) * 0.3048, 1)
            cle = (lon, lat, alt_m)
            if cle not in vus:
                coords.append(cle)
                vus.add(cle)
    return coords

def couleur_altitude(alt):
    if alt < 1000:
        return "ff00ff00"  # vert
    elif alt < 3000:
        return "ff00a5ff"  # orange
    else:
        return "ff0000ff"  # rouge

def generer_kml_segments(coordonnees, nom_fichier="segments_colores.kml"):
    with open(nom_fichier, "w", encoding="utf-8") as f:
        f.write('<?xml version="1.0" encoding="UTF-8"?>\n')
        f.write('<kml xmlns="http://www.opengis.net/kml/2.2">\n<Document>\n')

        # Style pour départ et arrivée
        f.write('  <Style id="debut"><IconStyle><color>ff00ff00</color><scale>1.2</scale>')
        f.write('<Icon><href>http://maps.google.com/mapfiles/kml/shapes/arrow.png</href></Icon>')
        f.write('</IconStyle></Style>\n')

        f.write('  <Style id="fin"><IconStyle><color>ffff0000</color><scale>1.2</scale>')
        f.write('<Icon><href>http://maps.google.com/mapfiles/kml/shapes/arrow.png</href></Icon>')
        f.write('</IconStyle></Style>\n')

        # Marqueur départ
        lon, lat, alt = coordonnees[0]
        f.write(f'''<Placemark>
  <name>Départ</name>
  <styleUrl>#debut</styleUrl>
  <Point><coordinates>{lon},{lat},{alt}</coordinates></Point>
</Placemark>\n''')

        # Marqueur arrivée
        lon, lat, alt = coordonnees[-1]
        f.write(f'''<Placemark>
  <name>Arrivée</name>
  <styleUrl>#fin</styleUrl>
  <Point><coordinates>{lon},{lat},{alt}</coordinates></Point>
</Placemark>\n''')

        # Segments colorés
        for i in range(len(coordonnees) - 1):
            lon1, lat1, alt1 = coordonnees[i]
            lon2, lat2, alt2 = coordonnees[i + 1]
            alt_moy = (alt1 + alt2) / 2
            couleur = couleur_altitude(alt_moy)
            f.write(f'''<Style id="line{i}">
  <LineStyle><color>{couleur}</color><width>3</width></LineStyle>
</Style>
<Placemark>
  <styleUrl>#line{i}</styleUrl>
  <LineString>
    <extrude>0</extrude>
    <tessellate>1</tessellate>
    <altitudeMode>absolute</altitudeMode>
    <coordinates>
      {lon1},{lat1},{alt1}
      {lon2},{lat2},{alt2}
    </coordinates>
  </LineString>
</Placemark>\n''')

        f.write('</Document>\n</kml>\n')

# Utilisation
with open("trame_f4kmn-11.txt", "r", encoding="utf-8") as fichier:
    lignes = fichier.readlines()


coordonnees = extraire_coordonnees(lignes)
coordonnees = supprimer_doublons(coordonnees)
generer_kml_segments(coordonnees)

