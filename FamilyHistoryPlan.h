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


typedef char* fhuColControlT;
#define FHA_COL_ID_LEN 3 // The length of the constants below.
#define FHA_ROW_IN_ROW 0 // The zero based target row. (advisory)
#define FHA_TYP_IN_ROW 1 // The zero based column type.
#define FHA_COL_IN_ROW 2 // The zero based column letter.

#define FHA_COL_PRINM "Pla" // full name
#define FHA_COL_PRIDT "Pde" // event ("w") date
#define FHA_COL_PRIPL "Plf" // event (from) place
#define FHA_COL_PRIGN "Plg" // gender
#define FHA_COL_MTHNM "Plm" // mother's (mama) full name
#define FHA_COL_FTHNM "Plp" // father's (papa) full name
#define FHA_COL_SPONM "Pls" // spouse's full name
#define FHA_COL_BTNBR "Plr" // batch_number (resource; occurs in batch index, name date index, and here)
#define FHA_COL_SYSID "Plx" // host's person Id, FS person url (name date index and here)
                             // Meta data
#define FHA_COL_MTSRCTP "Mlm" // source_media_type
#define FHA_COL_MTRECRL "Mlr" // role_in_record
#define FHA_COL_MTRLTHD "Mlh" // relationship_to_head
                           // Other data
#define FHA_COL_OTHCFNM "Olc" // Children's full names
#define FHA_COL_OTHOFNM "Olo" // Other people's full names
#define FHA_COL_OTHPFNM "Olp" // Parent's full names
#define FHA_COL_OTHEVNT "Ole" // Other events

/**
 * An Id for each column found across all sources.
 */
typedef enum universalColumnIdEnum
{
    UCI_SCORE = 1,  // X score (value becomes 100 for client records)
    UCI_BCHNBR,     // M batch_number
    UCI_FULLNM,     // P full_name
    UCI_GNDR,       // P gender
    UCI_BDT,        // P birth_date
    UCI_BPLC,       // P birth_place_text
    UCI_CDT,        // P chr_date
    UCI_CPLC,       // P chr_place_text
    UCI_RESDT,      // P residence_date
    UCI_RESPLC,     // P residence_place_text
    UCI_MDT,        // P marriage_date
    UCI_MPLC,       // P marriage_place_text
    UCI_DDT,        // P death_date
    UCI_DPLC,       // P death_place_text
    UCI_IDT,        // P burial_date
    UCI_IPLC,       // P burial_place_text
    UCI_FFNM,       // P father_full_name
    UCI_MFNM,       // P mother_full_name
    UCI_SFNM,       // P spouse_full_name
    UCI_PFNMS,      // O parent_full_names
    UCI_CFNMS,      // O children_full_names
    UCI_OFNMS,      // O other_full_names
    UCI_OEVENTS,    // O other_events
    UCI_SYSID,      // M the person Id from the host, FS=person_url
    UCI_EUID,       // X easy_unique_id
    UCI_SUBCLLID,   // X subcollection_id
    UCI_SMT,        // X source_media_type
    UCI_ROLEINREC,  // X role_in_record
    UCI_RELTOHEAD,  // X relationship_to_head
    UCI_RURL,       // X record_url
    UCI_MAX_FLD
}universalColumnIdET; // UCI

typedef char* nullTermNameListT[5];
typedef char* fieldValueT;

typedef struct FHU_ColunmAlternateNamesStruct
{
    universalColumnIdET uci;
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
#define FHXR_HDSTART 1000000
#define FHXRCH_BEGIN "score"
#define FHXR_OUTSZ       300


typedef struct FHU_ControlStruct
{
    char fileName[17+1];    // whoZ
    int  rowNbr;            // indeX
    int  colNbr;
    
    char *currWrite;
    char *currentRead;
    int   linePresentingError;
    int   droppedCount;
    char  buf[FHXR_BUF_SZ];
}FHU_controlT, *FHU_controlPT;

extern FHU_controlT FHU_control;

#endif /* FamilyHistoryPlan_h */
/**
 *
 */
