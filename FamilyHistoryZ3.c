//
//  FamilyHistoryZ3.c
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
#include "CursorO0.h"
#include "DictO0.h"
// data plans
#include "FamilyHistoryZ3Plan.h"
// other api's
#include "FamilyHistoryO3.h"

static Uint countOfData = -1;
static Uint countOfLink = -1;
#define FHZ3_IDNM_Z 10000
#define FHZ3_LINK_Z 10000
static FamilyHistoryZ1ProvidedIdNmDataT       FAProvidedIdNmData[FHZ3_IDNM_Z];
static FamilyHistoryZ1LinkDataT               FALinkData[FHZ3_LINK_Z];

FHZ0FilesACdataT FHZ0FilesACdata[] =
{
    {false, "fsmyl-core"     },
    {false, "famsch-V00732-1"},
    {false, "famsch-V00744-0"},
    {false, "famsch-V01412-8"},
    {false, "famsch-V01412-9"},
    {false, "famsch-V01413-1"},
    {false, "famsch-V01413-5"},
    {false, "famsch-V01414-3"},
    {false, "famsch-V01415-0"},
    {false, "famsch-V01415-1"},
    {false, "famsch-V01415-2"},
    {false, "famsch-V01415-3"},
    {false, "famsch-V01415-7"},
    {false, "famsch-V01416-0"},
    {false, "famsch-V01416-9"},
    {false, "famsch-V01417-2"},
    {false, "famsch-V01418-0"},
    {false, 0}
};

#define FHZ3_LOCPOOL_Z 10000
static char locationPool[FHZ3_LOCPOOL_Z];
static char* nextLocationP = &locationPool[0];

static void
FHZ3_init()
{
    countOfData = 0;
    countOfLink = 0;
    
    for(int initIx = 0; initIx < FHZ3_IDNM_Z; initIx++)
    {
        strcpy(FAProvidedIdNmData[initIx].fullName, "");
        strcpy(FAProvidedIdNmData[initIx].providedId, "");
        strcpy(FAProvidedIdNmData[initIx].batchId, "");
    }
    
    for(int initIx = 0; initIx < FHZ3_LINK_Z; initIx++)
    {
        FALinkData[initIx].eventType    = FHZ3_ET_AVAIL;
        FALinkData[initIx].score        = FHZ3_SCORE_AVAIL;
        
        //FALinkData[initIx].pvddIdNm;
        FALinkData[initIx].backPointer = 0;
        FALinkData[initIx].dateOf = FHZ3_NULL_DATE;
    }
}

/**
 * Many actons are taken to store the incoming data.
 */
static void
FHZ3_add(CursorO0HIthisPT curThisP /*providedIdPT pvdP, batchIdPT bchP, fullNamePT nmP, dateStrPT dtStrP, locationPT locP*/)
{
    // Work with the link first.
    countOfLink++;
    FALinkData[countOfLink].eventType    = FHZ3_ET_NONE;
    FALinkData[countOfLink].score        = FHZ3_SCORE_POOL;
    
    locationPT locP = ""; //curThisP->apiP->getField(&curThisP->data, "x"); TODO: locations by batchId
    
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
    dateStrPT uvslDtP = curThisP->apiP->getField(&curThisP->data, FHA_COLTP_UVSLDT + FHA_LTR_IN_ROW);
    FALinkData[countOfLink].dateOf = strtol(uvslDtP,  NO_ARG_PTR_ARC, RADIX_10_ARC);
    providedIdPT pvdP = curThisP->apiP->getField(&curThisP->data, FHA_COLTP_PRIDB + FHA_LTR_IN_ROW);
    // Work with the person second and as needed,
    FALinkData[countOfLink].pvddIdNmP = 0;
    for(int schIx = countOfLink - 1; schIx > 0; schIx--){
        if(FAProvidedIdNmData[schIx].providedId == pvdP){
            FALinkData[countOfLink].pvddIdNmP   = &FAProvidedIdNmData[schIx];
        }
    }
    
    batchIdPT bchP = curThisP->apiP->getField(&curThisP->data, FHA_COLTP_BTHID + FHA_LTR_IN_ROW);
    fullNamePT nmP = curThisP->apiP->getField(&curThisP->data, FHA_COLTP_PRINM + FHA_LTR_IN_ROW);
    
    // If an equal is not found then add this
    // otherwise link it to the match.
    if(FALinkData[countOfLink].pvddIdNmP == 0){
        countOfData++;
        strcpy(FAProvidedIdNmData[countOfData].batchId, bchP);
        strcpy(FAProvidedIdNmData[countOfData].providedId, pvdP);
        strcpy(FAProvidedIdNmData[countOfData].fullName, nmP);
        FALinkData[countOfLink].pvddIdNmP   = &FAProvidedIdNmData[countOfData];
    }
}

FamilyHistoryZ3ACapiT FamilyHistoryZ3ACapi =
{
    FHZ3_init,
    FHZ3_add
};

// END FamilyHistoryZ3.c
/**
 *
 */
