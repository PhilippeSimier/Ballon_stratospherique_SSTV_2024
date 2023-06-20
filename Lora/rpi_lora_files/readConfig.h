/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   readConfig.h
 * Author: ale
 *
 * Created on 10 juin 2023, 08:54
 */

#ifndef READCONFIG_H
#define READCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LoRa.h"
    
#define MAX_LINE_LENGTH 100

int readConfiguration(LoRa_ctl *modem, char *header);
BandWidth checkBandWidth(char *value);
SpreadingFactor checkSpreadingFactor(char *value);
ErrorCodingRate checkErrorCodingRate(char *value);

#ifdef __cplusplus
}
#endif

#endif /* READCONFIG_H */

