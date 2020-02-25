//
//  FamilyHistoryZ3TestAid.h
//  2020Mar10Dev
//
//  Created by Robert Russell Millward on 02/25/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
#ifndef FamilyHistoryZ3TestAid_h
#define FamilyHistoryZ3TestAid_h
// os
//#include <stdio.h>
// helper and service api's
// data plans
#include "FamilyHistoryZ0Plan.h"
#include "FamilyHistoryZ3Plan.h"
// other api's
#include "FamilyHistoryO0.h"
#include "FamilyHistoryO3.h"

typedef struct FamilyHistoryZ0TestAidClassApiStructure
{
    void (*setupSuite)(lineNbrT);
    bool (*isScheduled)(cfileNameT, lineNbrT);
    char* (*getAssertText)(cfuncNameT, lineNbrT);
    
    void (*setupSet1)(lineNbrT);
    bool (*doSet1)(lineNbrT);
    void (*teardownSet1)(lineNbrT);
    
    
    void (*setupSet2)(lineNbrT);
    bool (*doSet2)(lineNbrT);
    void (*teardownSet2)(lineNbrT);
    
    void (*setupSet3)(lineNbrT);
    bool (*doSet3)(lineNbrT);
    void (*teardownSet3)(lineNbrT);
    
    void (*setupSet4)(lineNbrT);
    bool (*doSet4)(lineNbrT);
    void (*teardownSet4)(lineNbrT);
    
    void (*teardownSuite)(lineNbrT);
}FHZ0QCapiT, *FHZ0QCapiPT;

extern FHZ0QCapiT FHZ0QCapi;


typedef struct FamilyHistoryZ3TestAidClassApiStructure
{
    void (*setupSuite)(lineNbrT);
    bool (*isScheduled)(cfileNameT, lineNbrT);
    char* (*getAssertText)(cfuncNameT, lineNbrT);
    
    void (*setupSet1)(lineNbrT);
    bool (*doSet1)(lineNbrT);
    void (*teardownSet1)(lineNbrT);
    
    
    void (*setupSet2)(lineNbrT);
    bool (*doSet2)(lineNbrT);
    void (*teardownSet2)(lineNbrT);
    
    void (*setupSet3)(lineNbrT);
    bool (*doSet3)(lineNbrT);
    void (*teardownSet3)(lineNbrT);
    
    void (*setupSet4)(lineNbrT);
    bool (*doSet4)(lineNbrT);
    void (*teardownSet4)(lineNbrT);
    
    void (*teardownSuite)(lineNbrT);
}FHZ3QCapiT, *FHZ3QCapiPT;

extern FHZ3QCapiT FHZ3QCapi;

#endif /* FamilyHistoryZ3TestAid_h */
/**
 * To test Z3 and Z0 from Z3 seems the most conservative plan.
 */
