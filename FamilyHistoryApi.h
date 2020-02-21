//
//  FamilyHistoryO0.h
//  2020Mar10Dev
//
//  Created by Robert Russell Millward on 2/15/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
#ifndef FamilyHistoryO0_h
#define FamilyHistoryO0_h
// os
// helper and service api's
// data plans
#include "FamilyHistoryPlan.h"
#include "ArchitectureZ0Plan.h"

typedef struct FHUApplicationClassApiStruct
{
    void (*newFile)(char* path, fileWoTypeT, gpSllgChar64PT);
}FHUO0ACapiT;

extern FHUO0ACapiT FHUO0ACapi;

#endif /* FamilyHistoryO0_h */
/**
 *
 */
