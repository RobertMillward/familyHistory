//
//  FamilyAnalyzePlan.h
//
//  Created by Robert Russell Millward on 2/19/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
#ifndef FamilyAnalyzePlan_h
#define FamilyAnalyzePlan_h
// os
#include <stdio.h>
#include "ArchitectureABugXC.h"
// helper and service api's
// data plans
#include "ArchitectureZ0Plan.h"
#include "FamilyHistoryPlan.h"

typedef enum FamilyAnalyzeNameDateHostIdEventTypeEnum
{
    FA_ET_AVAIL,
    FA_ET_ALSO_ME,
    FA_ET_PARENT,
    FA_ET_SPOUSE,
    
}FamilyAnalyzeNmDtIdEtEnumT;

typedef enum FamilyAnalyzeScoreEnum
{
    FA_SCORE_AVAIL,
    FA_SCORE_HIDE,
    FA_SCORE_POOL,
    FA_SCORE_MINE
}FamilyAnalyzeScoreEnumT;

#define FA_NULL_YEAR 0
#define FA_NULL_MONTH 0
#define FA_NULL_DAY 0
#define FA_NULL_DATE ((FA_NULL_YEAR * 100 * 100) + (FA_NULL_MONTH * 100) + FA_NULL_DAY)

typedef struct FamilyAnalyzeProvidedIdNameDataStruct
{
    uciProvidedIdT      uciProvidedId;
    uciBatchIdT         uciBatchId;
    uciFullNameT        uciFullName;
}FamilyAnalyzeProvidedIdNmDataT, *FamilyAnalyzeProvidedIdNmDataPT;

/**
 * These are stored in chronoligically reverse order so children come before parents and one spouse comes before the other.
 */
typedef struct FamilyAnalyzeLinkDataStruct
{
    FamilyAnalyzeProvidedIdNmDataPT                 pvddIdNm;
    struct FamilyAnalyzeNameDateHostIdDataStruct    *backPointer;   // another me or spouse or parent
    FamilyAnalyzeNmDtIdEtEnumT                      eventType;
    Ulng                                            dateOf;         // YYYYMMDD MM = 00, 01-12; DD = 00, 01-31
    FamilyAnalyzeScoreEnumT                         score;
    
}FamilyAnalyzeLinkDataT, *FamilyAnalyzeLinkDataPT;

typedef struct FamilyAnalyzeApplicationClassApiStruct
{
    void (*init)(void);
    void (*addSomething)(uciProvidedIdT, uciBatchIdT, uciFullNameT); // TODO more cols
    
}FamilyAnalyzeACapiT;

extern FamilyAnalyzeACapiT FamilyAnalyzeACapi;

#endif /* FamilyAnalyzePlan_h */
