/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
        return 1;
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

}


 BandWidth checkBandWidth(char *value){
   if (strcmp(value, "BW7_8") == 0) {
       return BW7_8;
   }  
   else if (strcmp(value, "BW10_4") == 0) {
       return BW10_4;
   }  
   else if (strcmp(value, "BW15_6") == 0) {
       return BW15_6;
   }  
   else if (strcmp(value, "BW20_8") == 0) {
       return BW20_8;
   }  
   else if (strcmp(value, "BW31_25") == 0) {
       return BW31_25;
   }  
   else if (strcmp(value, "BW41_7") == 0) {
       return BW41_7;
   }  
   else if (strcmp(value, "BW62_5") == 0) {
       return BW62_5;
   }  
   else if (strcmp(value, "BW125") == 0) {
       return BW125;
   }  
   else if (strcmp(value, "BW250") == 0) {
       return BW250;
   }  
   else if (strcmp(value, "BW500") == 0) {
       return BW500;
   }  
 }

SpreadingFactor checkSpreadingFactor(char *value){
   if (strcmp(value, "SF7") == 0) {
       return SF7;
   }  
   else if (strcmp(value, "SF8") == 0) {
       return SF8;
   }  
   else if (strcmp(value, "SF9") == 0) {
       return SF9;
   }  
   else if (strcmp(value, "SF10") == 0) {
       return SF10;
   }  
   else if (strcmp(value, "SF11") == 0) {
       return SF11;
   }  
   else if (strcmp(value, "SF12") == 0) {
       return SF12;
   }    
}


ErrorCodingRate checkErrorCodingRate(char *value){
   if (strcmp(value, "CR5") == 0) {
       return CR5;
   }  
   else if (strcmp(value, "CR6") == 0) {
       return CR6;
   }  
   else if (strcmp(value, "CR7") == 0) {
       return CR7;
   }  
   else if (strcmp(value, "CR8") == 0) {
       return CR8;
   }  
   
}
