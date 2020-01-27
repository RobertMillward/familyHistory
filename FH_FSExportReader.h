//
//  FH_FSExportReader.h
//  Repository:Apps/FamHist
//
//  Created by Robert R on 2/9/15.
//  Copyright (c) 2015 Robert Russell Millward.
//  All rights reserved.
//
/**
 * load and process a FamilySearch exported CSV.
 * It has to be done enmass because of the guaranteed duplicates.
 */
#ifndef __FH_FSExportReader__
#define __FH_FSExportReader__

#include "ArchitectureO0.h"
#include "ArchFamilyHistory.h"

typedef int fdT;

/**
 * The database table, column Id
 * (for Row.c field management).
 * Records generatable:
 * - P[erson]
 * - M[eta]
 * - O[ther]
 */
#define FHA_OUT_PRSN "P"
#define FHA_OUT_META "M"
#define FHA_OUT_OTHR "O"

#define FHA_COL_ID_LEN 2 // The length of the constants below.
#define FHA_COL_IN_ROW 1 // The zero based column letter.

#define FHA_COL_PRINM "Pa" // full name
#define FHA_COL_PRIGN "Pb" // gender
#define FHA_COL_PRIDT "Pc" // event ("w") date
#define FHA_COL_PRIPL "Pd" // event place
#define FHA_COL_MTHNM "Pe" // mother's full name
#define FHA_COL_FTHNM "Pf" // father's full name
#define FHA_COL_SPONM "Pg" // spouse's full name
                           // Other data
#define FHA_COL_OTHCFNM "Ob" // Children's full names
#define FHA_COL_OTHOFNM "Oc" // Other people's full names
#define FHA_COL_OTHPFNM "Od" // Parent's full names
#define FHA_COL_OTHEVNT "Oe" // Other events
                             // Meta data
#define FHA_COL_MTABTCH "Ma" // batch number
#define FHA_COL_MTAROLE "Mb" // pri. person's role
#define FHA_COL_MTARELT "Mc" // relationship
#define FHA_COL_MTAPURL "Md" // FS url

/**
 * [F]amily[H]istory e[X]port [R]eader parameters
 * - max buffer size
 * - heard start (data fill overlaps)
 * - column names start in file
 */
#define FHXR_BUF_SZ    50000
#define FHXR_HDSTART    5000
#define FHXRCH_BEGIN "score"
#define FHXR_OUTSZ       300

/**
 * This instance contains internal pointers
 * and thus can not be moved.
 */
typedef struct FHFSExportReaderS
{
    char *currWrite;
    char *currentRead;
    int   linePresentingError;
    int   droppedCount;
    char buf[FHXR_BUF_SZ];
    
}FHFSExportReader, *FHFSExportReaderP;

extern FHFSExportReader newFHFSExportReader(
            char *fullPath);

#endif /* defined(__FH_FSExportReader__) */
