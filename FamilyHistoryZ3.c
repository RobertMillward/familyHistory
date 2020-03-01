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
#include "ArchitectureZ0Plan.h"
// other api's
#include "FamilyHistoryO0.h"
#include "FamilyHistoryO3.h"

static Uint countOfData = -1;
static Uint countOfLink = -1;
#define FHZ3_IDNM_Z 10000
#define FHZ3_LINK_Z 10000
static FHZ3NameBatchAIdataT  FHZ3NameBatchAIdata[FHZ3_IDNM_Z];
static FHZ3LinkDataT         FHZ3LinkData[FHZ3_LINK_Z];



#define FHZ3_PLACEPOOL_Z 10000
static char placePool[FHZ3_PLACEPOOL_Z];
static char* nextLocationP = &placePool[0];

/**
 * See api definition
 */
static void
FHZ3_init(gpSllgChar64PT gp64P)
{
    countOfData = 0;
    countOfLink = 0;
    
    for(int initIx = 0; initIx < FHZ3_IDNM_Z; initIx++)
    {
        strcpy(FHZ3NameBatchAIdata[initIx].fullName, "");
        strcpy(FHZ3NameBatchAIdata[initIx].providedId, "");
        strcpy(FHZ3NameBatchAIdata[initIx].batchId, "");
    }
    
    for(int initIx = 0; initIx < FHZ3_LINK_Z; initIx++)
    {
        FHZ3LinkData[initIx].eventType    = FHZ3_ET_AVAIL;
        FHZ3LinkData[initIx].score        = FHZ3_SCORE_AVAIL;
        
        //FALinkData[initIx].pvddIdNm;
        FHZ3LinkData[initIx].backPointer = 0;
        FHZ3LinkData[initIx].dateOf = FHZ3_NULL_DATE;
    }
    
    
}

/**
 * Many actons are taken to store the incoming data.
 */
static void
FHZ3_add(CursorO0HIthisPT curThisP /*providedIdPT pvdP, batchIdPT bchP, fullNamePT nmP, dateStrPT dtStrP, placePT locP*/)
{
    // Work with the link first.
    countOfLink++;
    FHZ3LinkData[countOfLink].eventType    = FHZ3_ET_NONE;
    FHZ3LinkData[countOfLink].score        = FHZ3_SCORE_POOL;
    
    placePT placeP = ""; //curThisP->apiP->getField(&curThisP->data, ID_STR_ROW); TODO: places by batchId
    
    // place pool
    FHZ3LinkData[countOfLink].placeP = 0;
    for(char *schLocP = placePool; schLocP < nextLocationP ; )
    {
        if(strcmp(schLocP, placeP) == 0)
        {
            FHZ3LinkData[countOfLink].placeP = schLocP;
        }
    }
    if(FHZ3LinkData[countOfLink].placeP == 0){
        strcpy(nextLocationP, placeP);
        FHZ3LinkData[countOfLink].placeP   = nextLocationP;
        nextLocationP += strlen(nextLocationP) + 1;
    }
    
    // TODO: missing parts
    dateStrPT uvslDtP = curThisP->apiP->getField(&curThisP->data, FHA_COLTP_UVSLDT + FHA_LTR_IN_ROW);
    FHZ3LinkData[countOfLink].dateOf = strtol(uvslDtP,  NO_ARG_PTR_ARC, RADIX_10_ARC);
    providedIdPT pvdP = curThisP->apiP->getField(&curThisP->data, FHA_COLTP_PRIDB + FHA_LTR_IN_ROW);
    // Work with the person second and as needed,
    FHZ3LinkData[countOfLink].pvddIdNmP = 0;
    for(int schIx = countOfLink - 1; schIx > 0; schIx--){
        if(FHZ3NameBatchAIdata[schIx].providedId == pvdP){
            //FHZ3LinkData[countOfLink].pvddIdNmP   = &FHZ3NameBatchAIdata[schIx];
        }
    }
    
    batchIdPT bchP = curThisP->apiP->getField(&curThisP->data, FHA_COLTP_BTHID + FHA_LTR_IN_ROW);
    fullNamePT nmP = curThisP->apiP->getField(&curThisP->data, FHA_COLTP_PRINM + FHA_LTR_IN_ROW);
    
    // If an equal is not found then add this
    // otherwise link it to the match.
    if(FHZ3LinkData[countOfLink].pvddIdNmP == 0){
        countOfData++;
        strcpy(FHZ3NameBatchAIdata[countOfData].batchId, bchP);
        strcpy(FHZ3NameBatchAIdata[countOfData].providedId, pvdP);
        strcpy(FHZ3NameBatchAIdata[countOfData].fullName, nmP);
        //FHZ3LinkData[countOfLink].pvddIdNmP   = &FHZ3NameBatchAIdata[countOfData];
    }
}

FamilyHistoryZ3ACapiT FamilyHistoryZ3ACapi =
{
    FHZ3_init,
    FHZ3_add
};

// END FamilyHistoryZ3.c
/**
 */
