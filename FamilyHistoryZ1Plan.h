//
//  FamilyHistoryZ1Plan.h
//  2020Mar10Dev
//
//  Created by Robert Russell Millward on 2/19/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
#ifndef FamilyHistoryZ1Plan_h
#define FamilyHistoryZ1Plan_h
// os
#include <stdio.h>
#include "ArchitectureABugXC.h"
// helper and service api's
// data plans
#include "ArchitectureZ0Plan.h"
#include "FamilyHistoryZ0Plan.h"

/**
 * The primary index identifies relationships. It is not about events but they are used
 * to generate a link between two people. For example, each sibling attending a
 * parent's burial would have a link back to that parent. Another, assuming mother younger
 * than father, each child event would have a link to mama who in turn has a link to papa.
 * The data is sorted chronologically so age of each person can be used effectivly.
 * If a person is moved to or from a pool then everyone linked is also moved.
 */
typedef enum FamilyHistoryZ1NameDateHostIdEventTypeEnum
{
    FA_ET_AVAIL,
    FA_ET_NONE,     // End of the line
    FA_ET_ALSO_ME,  // Combine provided Ids or add data to an existing id
    FA_ET_PARENT,
    FA_ET_SPOUSE,
    
}FamilyHistoryZ1NmDtIdEtEnumT;

/**
 * Three piles of data exist. Everythin starts in the POOL. From the POOL records move to the pile that is MINE or the pile
 * that is HIDEN based on user commands.
 */
typedef enum FamilyHistoryZ1ScoreEnum
{
    FA_SCORE_AVAIL,
    FA_SCORE_HIDE,
    FA_SCORE_POOL,
    FA_SCORE_MINE
}FamilyHistoryZ1ScoreEnumT;

#define FA_NULL_YEAR 0
#define FA_NULL_MONTH 0
#define FA_NULL_DAY 0
#define FA_NULL_DATE ((FA_NULL_YEAR * 100 * 100) + (FA_NULL_MONTH * 100) + FA_NULL_DAY)

typedef struct FamilyHistoryZ1ProvidedIdNameDataStruct
{
    uciProvidedIdT      uciProvidedId;
    uciBatchIdT         uciBatchId;
    uciFullNameT        uciFullName;
}FamilyHistoryZ1ProvidedIdNmDataT, *FamilyHistoryZ1ProvidedIdNmDataPT;

/**
 * These are stored in chronoligically reverse order so children come before parents and one spouse comes before the other.
 */
typedef struct FamilyHistoryZ1LinkDataStruct
{
    FamilyHistoryZ1ProvidedIdNmDataPT                 pvddIdNmP;      // points to the data parts
    uciLocationPT                                   locationP;      // points to the place name
    struct FamilyHistoryZ1NameDateHostIdDataStruct    *backPointer;   // another me or spouse or parent
    FamilyHistoryZ1NmDtIdEtEnumT                      eventType;
    Ulng                                            dateOf;         // YYYYMMDD MM = 00, 01-12; DD = 00, 01-31
    FamilyHistoryZ1ScoreEnumT                         score;          // moves the data among the piles
    
}FamilyHistoryZ1LinkDataT, *FamilyHistoryZ1LinkDataPT;

/**
 * The family history analize class api
 */
typedef struct FamilyHistoryZ1ApplicationClassApiStruct
{
    void (*init)(void);
    void (*add)(uciProvidedIdPT, uciBatchIdPT, uciFullNamePT, uciDateStrPT, uciLocationPT);
    
}FamilyHistoryZ1ACapiT;

extern FamilyHistoryZ1ACapiT FamilyHistoryZ1ACapi;

#endif /* FamilyHistoryZ1Plan_h */
