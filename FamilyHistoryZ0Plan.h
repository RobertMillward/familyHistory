//
//  FamilyHistoryZ0Plan.h
//  2020Mar10Dev
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

typedef int     fdT;
typedef char*   fileWoTypeT; // I'll add the csv and this will become the source.

typedef char    uciBatchIdT[31+1];
typedef char    uciDateStrT[11+1];
typedef char    uciFullNameT[63+1];
typedef char    uciProvidedIdT[31+1]; // the Id that came with the record
typedef char    uciLocationT[63+1];

typedef char*   uciBatchIdPT;
typedef char*   uciDateStrPT;
typedef char*   uciFullNamePT;
typedef char*   uciProvidedIdPT;
typedef char*   uciLocationPT;
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
#define FHA_COL_PRIDB "Pdb" // event (begin) date
#define FHA_COL_PRIDE "Pde" // event end date
#define FHA_COL_PRIPL "Plf" // event (from) place
#define FHA_COL_PRIGN "Plg" // gender
#define FHA_COL_MTHNM "Plm" // mother's (mama) full name
#define FHA_COL_OTHNM "Plo" // other full name
#define FHA_COL_FTHNM "Plp" // father's (papa) full name
#define FHA_COL_BTNBR "Plr" // batch_number (resource)
#define FHA_COL_SPONM "Pls" // spouse's full name
#define FHA_COL_EVNTT "Plt" // event type (b,c d, i, h, etc)
#define UCI_COL_UVSLDT "Plu" // universal date (not in any input)
#define FHA_COL_PRVDID "Plx" // provided person Id, FS person url
                             // Meta data
#define FHA_COL_MTSRCTP "Mln" // source_media_type
#define FHA_COL_MTRECRL "Mlq" // role_in_record
#define FHA_COL_MTRLTHD "Mlv" // relationship_to_head
                           // Other data
#define FHA_COL_OTHCFNM "Olc" // Children's full names
#define FHA_COL_OTHPFNM "Olp" // Parent's full names
#define FHA_COL_OTHEVNT "Olh" // Other events

/**
 * The columns that are output but may not be in the input, i.e calculated columns.
 * Programming note: must be higher than the highest real csv column.
 */
#define UCI_CSVCOL_EVNT_NBR 33
#define UCI_CSVCOL_EVNT_LBL "event_type"
#define UCI_CSVCOL_UVSL_NBR 34
#define UCI_CSVCOL_UVSL_LBL "universal_date"

#define UCI_EVTTP_RES "r"
#define UCI_EVTTP_BIR "b"
#define UCI_EVTTP_CHR "c"
#define UCI_EVTTP_MAR "h"
#define UCI_EVTTP_DTH "d"
#define UCI_EVTTP_BRY "i"


/**
 * An Id for each column found across all sources.
 */
typedef enum universalColumnIdEnum
{
    UCI_NULL,
    UCI_PVDDID,     // M the provided person Id from the record provider, FS=person_url
    UCI_SCORE,      // X score (value becomes 100 for client records)
    UCI_BCHNBR,     // M batch_number
    UCI_UVSLDT,     // P the universal date
    UCI_FULLNM,     // P full_name
    UCI_GNDR,       // P gender
    UCI_FFNM,       // P father_full_name
    UCI_MFNM,       // P mother_full_name
    UCI_SFNM,       // P spouse_full_name
    UCI_EVTTP,      // P event_type
    UCI_RESDTB,     // P event_date (begin), residence_date
    UCI_RESPLC,     // P event_place, residence_place_text (#11)
    // nice to have
    UCI_BDT,        // P birth_date
    UCI_BPLC,       // P birth_place_text
    UCI_CDT,        // P chr_date
    UCI_CPLC,       // P chr_place_text
    UCI_MDT,        // P marriage_date
    UCI_MPLC,       // P marriage_place_text
    UCI_DDT,        // P death_date
    UCI_DPLC,       // P death_place_text
    UCI_IDT,        // P burial_date
    UCI_IPLC,       // P burial_place_text (#21)
    // completely optional
    UCI_PFNMS,      // O parent_full_names
    UCI_CFNMS,      // O children_full_names
    UCI_OFNMS,      // O other_full_names
    UCI_SMT,        // X source_media_type
    UCI_ROLEINREC,  // X role_in_record
    UCI_OEVENTS,    // O other_events (#27)
    // added by us
    UCI_RESDTE,     // P event_date_end
    UCI_RELTOHEAD,  // X relationship_to_head
    // from here on seems to be obsoleted in FamilySearch
    UCI_EUID,       // X easy_unique_id
    UCI_SUBCLLID,   // X subcollection_id
    UCI_RURL,       // X record_url
    UCI_MAX_FLD
}universalColumnIdET; // UCI

#define UCI_CSVCOL_ALTNMS_Z 5
typedef char* nullTermNameListT[UCI_CSVCOL_ALTNMS_Z];
typedef char* fieldValueT;          // pointer to the csv value before adding to the row.

typedef struct FHZ0ColunmAlternateNamesStruct
{
    universalColumnIdET uci;
    fhuColControlT      control;    // the primary target row code, type, and field letter.
    fieldValueT         value;
    massOfT             length;
    nullTermNameListT   list;       // the various csv column names for this universal column.
}FHZ0DictionaryAndValueT, *FHZ0DictionaryAndValuePT;



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


typedef struct FHZ0BufControlApplicationClassDataStruct
{
    char fileName[17+1];    // whoZ
    int  rowNbr;            // indeX
    int  colNbr;
    
    char *currWrite;
    char *currentRead;
    int   linePresentingError;
    int   droppedCount;
    char  buf[FHXR_BUF_SZ];
}FHZ0bufControlACdataT, *FHZ0bufControlACdataPT;

extern FHZ0bufControlACdataT FHZ0control; // TOTO: rename structure instance

#endif /* FamilyHistoryZ0Plan_h */
/**
 * Level 0 is the importing of the source which is then exported according to FHZ0controlAIdata.
 */
