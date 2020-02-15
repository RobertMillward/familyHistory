//
//  FH_FSExportReader.h
//
//  Created by Robert R on 2/9/15.
//  Copyright (c) 2020 Robert Russell Millward.  All rights reserved.
//
#ifndef FH_FSExportReader_h
#define FH_FSExportReader_h
// os
// helper and service api's
// data plans
#include "ArchitectureZ0Plan.h"
#include "ArchFamilyHistory.h"
#include "FamilyHistoryPlan.h"



extern FHFSExportReader newFHFSExportReader(char *fullPath);

#endif /* FH_FSExportReader_h) */
/**
 * load and process a FamilySearch exported CSV.
 * It has to be done enmass because of the guaranteed duplicates.
 */
