//
//  FamilyHistoryApi.h
//  2020Feb20Dev
//
//  Created by Robert Russell Millward on 2/15/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
#ifndef FamilyHistoryApi_h
#define FamilyHistoryApi_h
// os
// helper and service api's
// data plans
#include "FamilyHistoryPlan.h"
#include "ArchitectureZ0Plan.h"

typedef struct FHUHelperClassApiStruct
{
    void (*newFile)(char* path, fileWoTypeT, gpSllgChar64PT);
}FHU_HCapiT;

extern FHU_HCapiT FHU_HCapi;

#endif /* FamilyHistoryApi_h */
