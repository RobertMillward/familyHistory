//
//  FamilyAnalyzePlan.c
//
//  Created by Robert Russell Millward on 2/19/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
// os
#include <string.h>
#include "ArchitectureABugXC.h"
// helper and service api's
// data plans
#include "FamilyAnalyzePlan.h"

static Uint countOfData = -1;
static Uint countOfLink = -1;
#define FAIDNM_Z 10000
#define FALINK_Z 10000
static FamilyAnalyzeProvidedIdNmDataT       FAProvidedIdNmData[FAIDNM_Z];
static FamilyAnalyzeLinkDataT               FALinkData[10000];

static void
FAinit()
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

FamilyAnalyzeACapiT FamilyAnalyzeACapi =
{
    FAinit
};
