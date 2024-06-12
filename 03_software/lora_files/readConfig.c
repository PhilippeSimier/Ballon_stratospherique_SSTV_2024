#include "readConfig.h"


int readConfiguration(LoRa_ctl *modem, char* header) {

   FILE *file;
    char line[MAX_LINE_LENGTH];
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];

    // Ouvrir le fichier INI en mode lecture
    file = fopen("/home/ballon/lora_files/config.ini", "r");
    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier config.ini\n");
        return -1;
    }

    // Lire chaque ligne du fichier
    while (fgets(line, sizeof(line), file)) {
        // Ignorer les lignes vides et les commentaires
        if (line[0] == '\n' || line[0] == ';') {
            continue;
        }

        // Extraire la clé et la valeur
        sscanf(line, "%[^=]=%s", key, value);
        // test des clés
        if (strcmp(key, "spiCS") == 0) {
            modem->spiCS = atoi(value);
        } else if (strcmp(key, "bw") == 0) {
            modem->eth.bw=checkBandWidth(value);
        } else if (strcmp(key, "sf") == 0) {
            modem->eth.sf=checkSpreadingFactor(value);
        } else if (strcmp(key, "ecr") == 0) {
            modem->eth.ecr=checkErrorCodingRate(value);
        } else if (strcmp(key, "CRC") == 0) {
            modem->eth.CRC = atoi(value);
        } else if (strcmp(key, "freq") == 0) {
            modem->eth.freq = atof(value);
        } else if (strcmp(key, "resetGpioN") == 0) {
            modem->eth.resetGpioN = atoi(value);
        } else if (strcmp(key, "dio0GpioN") == 0) {
            modem->eth.dio0GpioN = atoi(value);
        }
        else if (strcmp(key, "header") == 0) {
            strcpy(header,value);
        }

    }

    // Fermer le fichier
    fclose(file);
    return 0;
}


BandWidth checkBandWidth(char *value){

   BandWidth largeur = BW125;

   if (strcmp(value, "BW7_8") == 0) {
       largeur =  BW7_8;
   }
   else if (strcmp(value, "BW10_4") == 0) {
       largeur =  BW10_4;
   }
   else if (strcmp(value, "BW15_6") == 0) {
       largeur =  BW15_6;
   }
   else if (strcmp(value, "BW20_8") == 0) {
       largeur =  BW20_8;
   }
   else if (strcmp(value, "BW31_25") == 0) {
       largeur =  BW31_25;
   }
   else if (strcmp(value, "BW41_7") == 0) {
       largeur = BW41_7;
   }
   else if (strcmp(value, "BW62_5") == 0) {
       largeur = BW62_5;
   }
   else if (strcmp(value, "BW125") == 0) {
       largeur =  BW125;
   }
   else if (strcmp(value, "BW250") == 0) {
       largeur =  BW250;
   }
   else if (strcmp(value, "BW500") == 0) {
       largeur =  BW500;
   }
   return largeur;
}

SpreadingFactor checkSpreadingFactor(char *value){

   SpreadingFactor facteur = SF12;

   if (strcmp(value, "SF7") == 0) {
       facteur = SF7;
   }
   else if (strcmp(value, "SF8") == 0) {
       facteur = SF8;
   }
   else if (strcmp(value, "SF9") == 0) {
       facteur = SF9;
   }
   else if (strcmp(value, "SF10") == 0) {
       facteur = SF10;
   }
   else if (strcmp(value, "SF11") == 0) {
       facteur = SF11;
   }
   else if (strcmp(value, "SF12") == 0) {
       facteur = SF12;
   }
   return facteur;
}


ErrorCodingRate checkErrorCodingRate(char *value){

   ErrorCodingRate taux_correction = CR8;

   if (strcmp(value, "CR5") == 0) {
       taux_correction = CR5;
   }
   else if (strcmp(value, "CR6") == 0) {
       taux_correction = CR6;
   }
   else if (strcmp(value, "CR7") == 0) {
       taux_correction = CR7;
   }
   else if (strcmp(value, "CR8") == 0) {
       taux_correction = CR8;
   }
   return taux_correction;
}
