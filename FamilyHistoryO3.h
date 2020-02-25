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
 * The family history analyze level class api 
 */
typedef struct FamilyHistoryZ3ApplicationClassApiStruct
{
    /**
     * Initialize the internal structures.
     * Load the unique batchId places from all the selected files.
     * Load the searchable data.
     * Load the clues
     */
    void (*init)(gpSllgChar64PT);
    /**
     * Add one row to the array.
     */
    void (*add)(CursorO0HIthisPT /*TODO locationPT*/);
    
}FamilyHistoryZ3ACapiT, *FamilyHistoryZ3ACapiPT;

extern FamilyHistoryZ3ACapiT FamilyHistoryZ3ACapi;

#endif /* FamilyHistoryO3_h */
