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
#include "FamilyHistoryZ0Plan.h"
#include "ArchitectureZ0Plan.h"

typedef struct FHUApplicationClassApiStruct
{
    void (*newFile)(char* path, fileWoTypeT, gpSllgChar64PT);
}FHO0ACapiT;

extern FHO0ACapiT FHO0ACapi;

#endif /* FamilyHistoryO0_h */
/**
 *
 */
