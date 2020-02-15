//
//  FH_FSExportReader.h
//  Repository:Apps/FamHist
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

#define FHA_COL_ID_LEN 3 // The length of the constants below.
#define FHA_TYP_IN_ROW 1 // The zero based column type.
#define FHA_COL_IN_ROW 2 // The zero based column letter.

#define FHA_COL_PRINM "Pla" // full name
#define FHA_COL_PRIGN "Plb" // gender
#define FHA_COL_PRIDT "Plc" // event ("w") date
#define FHA_COL_PRIPL "Pld" // event place
#define FHA_COL_MTHNM "Ple" // mother's full name
#define FHA_COL_FTHNM "Plf" // father's full name
#define FHA_COL_SPONM "Plg" // spouse's full name
                           // Other data
#define FHA_COL_OTHCFNM "Olb" // Children's full names
#define FHA_COL_OTHOFNM "Olc" // Other people's full names
#define FHA_COL_OTHPFNM "Old" // Parent's full names
#define FHA_COL_OTHEVNT "Ole" // Other events
                             // Meta data
#define FHA_COL_MTABTCH "Mla" // batch number
#define FHA_COL_MTAROLE "Mlb" // pri. person's role
#define FHA_COL_MTARELT "Mlc" // relationship
#define FHA_COL_MTAPURL "Mld" // FS url

/**
 * [F]amily[H]istory e[X]port [R]eader parameters
 * - max buffer size
 * - heard start (data fill overlaps)
 * - column names start in file
 */
#define FHXR_BUF_SZ  5000000
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

extern FHFSExportReader newFHFSExportReader(char *fullPath);

#endif /* FH_FSExportReader_h) */
/**
 * load and process a FamilySearch exported CSV.
 * It has to be done enmass because of the guaranteed duplicates.
 */
