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

def dms_vers_decimal(degres_minutes, direction):
    degres = int(degres_minutes[:2])
    minutes = float(degres_minutes[2:])
    decimal = degres + minutes / 60
    if direction in ['S', 'W', 'O']:
        decimal = -decimal
    return decimal

def extraire_coordonnees(lignes):
    coords = []
    for ligne in lignes:
        match = re.search(r'!(\d{4}\.\d{2})([NS])\/(\d{5}\.\d{2})([EO])[^A]*A=(\d+)', ligne)
        if match:
            lat_dm, lat_dir, lon_dm, lon_dir, alt_ft = match.groups()
            lat = dms_vers_decimal(lat_dm, lat_dir)
            lon = dms_vers_decimal(lon_dm, lon_dir)
            alt_m = round(int(alt_ft) * 0.3048, 1)  # pieds → mètres
            coords.append((lon, lat, alt_m))
    return coords

def generer_kml_ligne(coordonnees, nom_fichier="trace_continue.kml"):
    with open(nom_fichier, "w", encoding="utf-8") as f:
        f.write('<?xml version="1.0" encoding="UTF-8"?>\n')
        f.write('<kml xmlns="http://www.opengis.net/kml/2.2">\n')
        f.write('<Document>\n')
        f.write('<name>Trajectoire continue</name>\n')
        f.write('<Style id="ligneRouge">\n')
        f.write('  <LineStyle>\n')
        f.write('    <color>ff0000ff</color>\n')  # rouge opaque
        f.write('    <width>3</width>\n')
        f.write('  </LineStyle>\n')
        f.write('</Style>\n')
        f.write('<Placemark>\n')
        f.write('  <name>Trajectoire</name>\n')
        f.write('  <styleUrl>#ligneRouge</styleUrl>\n')
        f.write('  <LineString>\n')
        f.write('    <extrude>0</extrude>\n')  # pas de barre verticale
        f.write('    <tessellate>1</tessellate>\n')
        f.write('    <altitudeMode>absolute</altitudeMode>\n')
        f.write('    <coordinates>\n')
        for lon, lat, alt in coordonnees:
            f.write(f'      {lon},{lat},{alt}\n')
        f.write('    </coordinates>\n')
        f.write('  </LineString>\n')
        f.write('</Placemark>\n')
        f.write('</Document>\n</kml>\n')

# Utilisation
with open("trame_f4kmn-11.txt", "r", encoding="utf-8") as fichier:
    lignes = fichier.readlines()


coordonnees = extraire_coordonnees(lignes)
coordonnees = supprimer_doublons(coordonnees)
generer_kml_ligne(coordonnees)


