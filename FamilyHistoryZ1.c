//
//  FamilyHistoryZ1.c
//  2020Mar10Dev
//
//  Created by Robert Russell Millward on 2/19/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
// os
#include <string.h>
#include <stdlib.h>
#include "ArchitectureABugXC.h"
// helper and service api's
// data plans
#include "FamilyHistoryZ1Plan.h"

static Uint countOfData = -1;
static Uint countOfLink = -1;
#define FAIDNM_Z 10000
#define FALINK_Z 10000
static FamilyHistoryZ1ProvidedIdNmDataT       FAProvidedIdNmData[FAIDNM_Z];
static FamilyHistoryZ1LinkDataT               FALinkData[FALINK_Z];

#define FALOCPOOL_Z 10000
static char locationPool[FALOCPOOL_Z];
static char* nextLocationP = &locationPool[0];

static void
FHZ1init()
{
    countOfData = 0;
    countOfLink = 0;
    
    for(int initIx = 0; initIx < FAIDNM_Z; initIx++)
    {
        strcpy(FAProvidedIdNmData[initIx].uciFullName, "");
        strcpy(FAProvidedIdNmData[initIx].uciProvidedId, "");
        strcpy(FAProvidedIdNmData[initIx].uciBatchId, "");
    }
    
    for(int initIx = 0; initIx < FALINK_Z; initIx++)
    {
        FALinkData[initIx].eventType    = FA_ET_AVAIL;
        FALinkData[initIx].score        = FA_SCORE_AVAIL;
        
        //FALinkData[initIx].pvddIdNm;
        FALinkData[initIx].backPointer = 0;
        FALinkData[initIx].dateOf = FA_NULL_DATE;
    }
}

/**
 * Many actons are taken to store the incoming data.
 */
static void
FHZ1add(uciProvidedIdPT pvdP, uciBatchIdPT bchP, uciFullNamePT nmP, uciDateStrPT dtStrP, uciLocationPT locP)
{
    // Work with the link first.
    countOfLink++;
    FALinkData[countOfLink].eventType    = FA_ET_NONE;
    FALinkData[countOfLink].score        = FA_SCORE_POOL;
    
    // location pool
    FALinkData[countOfLink].locationP = 0;
    for(char *schLocP = locationPool; schLocP < nextLocationP ; )
    {
        if(strcmp(schLocP, locP) == 0)
        {
            FALinkData[countOfLink].locationP = schLocP;
        }
    }
    if(FALinkData[countOfLink].locationP == 0){
        strcpy(nextLocationP, locP);
        FALinkData[countOfLink].locationP   = nextLocationP;
        nextLocationP += strlen(nextLocationP) + 1;
    }
    
    // TODO: missing parts
    FALinkData[countOfLink].dateOf  = strtol(dtStrP + 0, NO_ARG_PTR_ARC, RADIX_10_ARC) *           1;
    FALinkData[countOfLink].dateOf += strtol(dtStrP + 6, NO_ARG_PTR_ARC, RADIX_10_ARC) *   100 * 100;
    int monNbr = 0;
    switch(dtStrP[3] + dtStrP[4] + dtStrP[5])
    {
        case 'J' + 'a' + 'n': monNbr =  1; break;
        case 'F' + 'e' + 'b': monNbr =  2; break;
        case 'M' + 'a' + 'r': monNbr =  3; break;
        case 'A' + 'p' + 'r': monNbr =  4; break;
        case 'M' + 'a' + 'y': monNbr =  5; break;
        case 'J' + 'u' + 'n': monNbr =  6; break;
        case 'J' + 'u' + 'l': monNbr =  7; break;
        case 'A' + 'u' + 'g': monNbr =  8; break;
        case 'S' + 'e' + 'p': monNbr =  9; break;
        case 'O' + 'c' + 't': monNbr = 10; break;
        case 'N' + 'o' + 'v': monNbr = 11; break;
        case 'D' + 'e' + 'c': monNbr = 12; break;
        case 'U' + 'n' + 'k': monNbr =  0; break;
    }
    FALinkData[countOfLink].dateOf += monNbr * 100;
    
    // Work with the person second and as needed,
    FALinkData[countOfLink].pvddIdNmP = 0;
    for(int schIx = countOfLink - 1; schIx > 0; schIx--){
        if(FAProvidedIdNmData[schIx].uciProvidedId == pvdP){
            FALinkData[countOfLink].pvddIdNmP   = &FAProvidedIdNmData[schIx];
        }
    }
    
    // If an equal is not found then add this
    // otherwise link it to the match.
    if(FALinkData[countOfLink].pvddIdNmP == 0){
        countOfData++;
        strcpy(FAProvidedIdNmData[countOfData].uciBatchId, bchP);
        strcpy(FAProvidedIdNmData[countOfData].uciProvidedId, pvdP);
        strcpy(FAProvidedIdNmData[countOfData].uciFullName, nmP);
        FALinkData[countOfLink].pvddIdNmP   = &FAProvidedIdNmData[countOfData];
    }
}

FamilyHistoryZ1ACapiT FamilyHistoryZ1ACapi =
{
    FHZ1init,
    FHZ1add
};

// END FamilyHistoryZ1.c
/**
 *
 */
