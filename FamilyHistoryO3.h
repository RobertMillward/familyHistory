//
//  FamilyHistoryO3.h
//  2020Mar10Dev
//
//  Created by Robert Russell Millward on 2/23/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
#ifndef FamilyHistoryO3_h
#define FamilyHistoryO3_h
// other api's
#include "CursorO0.h"

/**
 * The family history analyze class api 
 */
typedef struct FamilyHistoryZ3ApplicationClassApiStruct
{
    void (*init)(void);
    /**
     * Add one row to the array.
     */
    void (*add)(CursorO0HIthisPT /*TODO locationPT*/);
    
}FamilyHistoryZ3ACapiT, *FamilyHistoryZ3ACapiPT;

extern FamilyHistoryZ3ACapiT FamilyHistoryZ3ACapi;

#endif /* FamilyHistoryO3_h */
