//
//  FamilyHistoryPlan.h
//
//  Created by Robert Russell Millward on 2/15/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
#ifndef FamilyHistoryPlan_h
#define FamilyHistoryPlan_h
// os
// helper and service api's
// data plans
#include "ArchitectureZ0Plan.h"

typedef int fdT;
typedef char* fileWoTypeT; // I'll add the csv and this will become the source.

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
#define FHA_ROW_IN_ROW 0 // The zero based column type.
#define FHA_TYP_IN_ROW 1 // The zero based column type.
#define FHA_COL_IN_ROW 2 // The zero based column letter.

#define FHA_COL_PRINM "Pla" // full name
#define FHA_COL_PRIGN "Plb" // gender
#define FHA_COL_PRIDT "Plc" // event ("w") date
#define FHA_COL_PRIPL "Pld" // event place
#define FHA_COL_MTHNM "Ple" // mother's full name
#define FHA_COL_FTHNM "Plf" // father's full name
#define FHA_COL_SPONM "Plg" // spouse's full name
#define FHA_COL_BTNBR "Plh" // batch_number
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
#define FHA_COL_MTSRCTP "Mlf" // source_media_type
#define FHA_COL_MTRECRL "Mlg" // role_in_record
#define FHA_COL_MTRLTHD "Mlh" // relationship_to_head

typedef char* fhuColControlT;

/**
 * An Id for each column found across all sources.
 */
typedef enum columnIdUniversalEnum
{
    CIU_SCORE = 1,  // X score
    CIU_IURL,       // M person_url
    CIU_BCHNBR,     // Z batch_number
    CIU_ROLEINREC,  // X role_in_record
    CIU_RELTOHEAD,  // X relationship_to_head
    CIU_RURL,       // X record_url
    CIU_FNAME,      // P full_name
    CIU_GNDR,       // P gender
    CIU_BDT,        // P birth_date
    CIU_BPLC,       // P birth_place_text
    CIU_CDT,        // P chr_date
    CIU_CPLC,       // P chr_place_text
    CIU_RESDT,      // P residence_date
    CIU_RESPLC,     // P residence_place_text
    CIU_MDT,        // P marriage_date
    CIU_MPLC,       // P marriage_place_text
    CIU_DDT,        // P death_date
    CIU_DPLC,       // P death_place_text
    CIU_IDT,        // P burial_date
    CIU_IPLC,       // P burial_place_text
    CIU_FFNM,       // P father_full_name
    CIU_MFNM,       // P mother_full_name
    CIU_SFNM,       // P spouse_full_name
    CIU_PFNMS,      // O parent_full_names
    CIU_CFNMS,      // O children_full_names
    CIU_OFNMS,      // O other_full_names
    CIU_OEVENTS,    // O other_events
    CIU_EUID,       // X easy_unique_id
    CIU_SUBCLLID,   // X subcollection_id
    CIU_SMT,        // X source_media_type
    CIU_MAX_FLD
}columnIdUniversalET; // Ciu

typedef char* nullTermNameListT[5];
typedef char* fieldValueT;

typedef struct FHU_ColunmAlternateNamesStruct
{
    columnIdUniversalET ciu;
    fhuColControlT      control;
    fieldValueT         value;
    massOfT             length;
    nullTermNameListT   list;
}FHU_DictionaryAndValueT, *FHU_DictionaryAndValuePT;



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


typedef struct FHU_ControlStruct
{
    char fileName[17+1];    // whoZ
    int rowNbr;             // indeX
    int colNbr;
    
    char *currWrite;
    char *currentRead;
    int   linePresentingError;
    int   droppedCount;
    char buf[FHXR_BUF_SZ];
}FHU_controlT, *FHU_controlPT;

extern FHU_controlT FHU_control;

#endif /* FamilyHistoryPlan_h */
/**
 *
 */
