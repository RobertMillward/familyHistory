//
//  FamilyHistoryZ0.c
//  2020Mar10Dev
//
//  Created by Robert Russell Millward on 2/15/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
// os
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ArchitectureAbugXC.h"
// helper and service api's
#include "HashBasicsO0.h"
#include "DictO0.h"
// data plans
#include "ArchitectureZ0Plan.h"
#include "FamilyHistoryZ0Plan.h"
#include "HashBasicsZ0Plan.h"
#include "RowZ0Plan.h"
#include "DictZ3Plan.h" // TODO: lower something
// application api's
#include "FamilyHistoryO0.h"

#define VALHERE 0
#define LENHERE 0

static FHZ0DictionaryAndValueT FHZ0DictionaryAndValue[] = // dnv
{
    {UCI_FULLNM,    FHA_COLTP_PRINM,   VALHERE, LENHERE, {"full_name",          0}},
    {FHA_COLID_PVDDID,    FHA_COLTP_PRVDID,  VALHERE, LENHERE, {"providedId", "person_url", 0}},
    {UCI_EVTTP,     FHA_COLTP_EVNTT,   VALHERE, LENHERE, {FHA_CSVCOL_EVNT_LBL,  0}},
    {UCI_UVSLDT,    FHA_COLTP_UVSLDT,  VALHERE, LENHERE, {UCI_CSVCOL_UVDT_LBL,  0}},
    {UCI_GNDR,      FHA_COLTP_PRIGN,   VALHERE, LENHERE, {"gender",             0}},
    {UCI_BDT,       FHA_COLTP_PRIDB,   VALHERE, LENHERE, {"birth_date",         0}},
    {UCI_BPLC,      FHA_COLTP_PRIPL,   VALHERE, LENHERE, {"birth_place_text",   0}},
    {UCI_CDT,       FHA_COLTP_PRIDB,   VALHERE, LENHERE, {"chr_date",           0}},
    {UCI_CPLC,      FHA_COLTP_PRIPL,   VALHERE, LENHERE, {"chr_place_text",     0}},
    {UCI_MDT,       FHA_COLTP_PRIDB,   VALHERE, LENHERE, {"marriage_date",      0}},
    {UCI_MPLC,      FHA_COLTP_PRIPL,   VALHERE, LENHERE, {"marriage_place_text",0}},
    {UCI_DDT,       FHA_COLTP_PRIDB,   VALHERE, LENHERE, {"death_date",         0}}, // TODO: make end date
    {UCI_DPLC,      FHA_COLTP_PRIPL,   VALHERE, LENHERE, {"death_place_text",   0}},
    {UCI_IDT,       FHA_COLTP_PRIDB,   VALHERE, LENHERE, {"burial_date",        0}}, // TODO: make end date
    {UCI_IPLC,      FHA_COLTP_PRIPL,   VALHERE, LENHERE, {"burial_place_text",  0}},
    
    {UCI_FFNM,      FHA_COLTP_FTHNM,   VALHERE, LENHERE, {"father_full_name",   0}},
    {UCI_MFNM,      FHA_COLTP_MTHNM,   VALHERE, LENHERE, {"mother_full_name",   0}},
    {UCI_SFNM,      FHA_COLTP_SPONM,   VALHERE, LENHERE, {"spouse_full_name",   0}},
    {UCI_PFNMS,     FHA_COLTP_PFNM,    VALHERE, LENHERE, {"parent_full_names",  0}},
    {UCI_CFNMS,     FHA_COLTP_OTHCFNM, VALHERE, LENHERE, {"children_full_names",0}},
    {FHA_COLID_BCHID, FHA_COLTP_BTHID, VALHERE, LENHERE, {"batch_number",       0}},
    {FHA_COLID_SCORE, FHA_COLTP_SCORE, VALHERE, LENHERE, {FHXRCH_BEGIN,         0}},
    
    {UCI_RESDTB,    FHA_COLTP_PRIDB,   VALHERE, LENHERE, {"event_date", "event_begin", "residence_date",       0}},
    {UCI_RESDTE,    FHA_COLTP_PRIDE,   VALHERE, LENHERE, {"event_end",   0}},
    {UCI_RESPLC,    FHA_COLTP_PRIPL,   VALHERE, LENHERE, {"event_place", "residence_place_text", 0}},
    {UCI_OFNMS,     FHA_COLTP_OTHNM,   VALHERE, LENHERE, {"other_full_names",     0}},
    {UCI_OEVENTS,   FHA_COLTP_OTHEVNT, VALHERE, LENHERE, {"other_events",         0}},
    
    {UCI_SMT,       FHA_COLTP_MTSRCTP, VALHERE, LENHERE, {"source_media_type",  0}},
    {UCI_ROLEINREC, FHA_COLTP_MTRECRL, VALHERE, LENHERE, {"role_in_record",     0}},
    {UCI_RELTOHEAD, FHA_COLTP_MTRLTHD, VALHERE, LENHERE, {"relationship_to_head", 0}},
    {UCI_SUBCLLID,  FHA_COLTP_MTSRCTP, VALHERE, LENHERE, {"subcollection_id",   0}},
    {UCI_EUID,      FHA_COLTP_MTSRCTP, VALHERE, LENHERE, {"easy_unique_id",     0}},
    {UCI_RURL,      FHA_COLTP_MTSRCTP, VALHERE, LENHERE, {"record_url",         0}},
    {0,             FHA_CATEGORY,      0,       0,       {"category",           0}} // UCI_META is 1-based
};

FHZ0bufControlACdataT FHZ0control;

/**
 * Upon locating in the above FHZ0DictionaryAndValue table the column name found in the import
 * store the index here for quick retrieval by column number.
 */
#define UCI_TODNV_Z 100
static int colToDnv[UCI_TODNV_Z];
/**
 * Upon locating in the above FHZ0DictionaryAndValue table the column name found in the import
 * store in [columnIdUniversalET value] the index so that the Ciu enums can quickly access the table.
 */
static int uciToDnv[UCI_TODNV_Z];

/**
 * A list of duplicated information to exclude from the pool
 * The content is =tD=x<pvddid>
 */
static char FHZ0duplicatedIds[50000] = "";
static char* FHZ0duplicatedIdsP = FHZ0duplicatedIds;

/**
 * A list of family information to modify for the pool
 * The content is =tF=x<pvddId>=i<score>=u<unvslDt>.
 * As family members are recommended and approved then their <unvslDt><score>
 * might look like:
 * <15640303><100.01><John><findSp><pvddid01>
 * <15640303><100.02><Mary><findSp><pvddid02>
 * <15640303><100.05><Billie><findMa><pvddid03>
 * <15640303><100.05><Billie><findPa><pvddid04>
 * <15640303><100.10><Jillie><findPa><pvddid05>
 * <15640303><100.10><Jillie><findMa><pvddid06>
 * Extra note: in the summaryreport duplicates at this level will be combined thus
 * eliminating the clutter of multiple, for instance, findPa records for an individual.
 */
static char FHZ0familyIds[500000] = "";
static char* FHZ0familyIdsP = FHZ0familyIds;

/**
 * This happens only upon finding a column header row in the import.
 * A return 0f -1 means the column header text was not found.
 */
static int
FHO0_checkColName(int fieldCtr, char *labelP)
{
    if(fieldCtr == 0){
        // initialize colToDnv, uciToDnv, and Dnv
        for(int initIx = 0; initIx < UCI_TODNV_Z; initIx++){
            uciToDnv[initIx] = UCI_CATEGORY; // a valid null for this table
            colToDnv[initIx] = -1;
        }
        for(int dnvIx = 0; FHZ0DictionaryAndValue[dnvIx].uci != UCI_CATEGORY; dnvIx++){
            FHZ0DictionaryAndValue[dnvIx].length = 0;
            FHZ0DictionaryAndValue[dnvIx].value = 0;
        }
        // These two necessary outputs may not be in the import.
        FHO0_checkColName(FHA_CSVCOL_EVNT_NBR, FHA_CSVCOL_EVNT_LBL);
        FHO0_checkColName(UCI_CSVCOL_UVDT_NBR, UCI_CSVCOL_UVDT_LBL);
    }
    
    int dnvIx = 0;
    for(; FHZ0DictionaryAndValue[dnvIx].uci != UCI_CATEGORY; dnvIx++)
    {
        for(int altNmIx = 0;
            FHZ0DictionaryAndValue[dnvIx].list[altNmIx] != 0 &&
            altNmIx < UCI_CSVCOL_ALTNMS_Z;
            altNmIx++){
            if(strncmp(FHZ0DictionaryAndValue[dnvIx].list[altNmIx],
                       labelP,
                       strlen(FHZ0DictionaryAndValue[dnvIx].list[altNmIx])) == 0){
                // initialize this column's redirection elements
                uciToDnv[FHZ0DictionaryAndValue[dnvIx].uci] = dnvIx;
                colToDnv[fieldCtr] = dnvIx;
                
                break;
            }
        }
        if(colToDnv[fieldCtr] == dnvIx){
            break;
        }
    }
    
    if(FHZ0DictionaryAndValue[dnvIx].uci == 0){
        dnvIx = -1;
    }

    return dnvIx;
}


static void
FHO0_OpenReadClose(char *path, fileWoTypeT fileWoType, gpSllgChar64PT gp64P)
{
    FHZ0bufControlACdataPT ctrlP = &FHZ0control;

    strcpy(ctrlP->fileName, fileWoType);
    
    char fullPath[1001];
    sprintf(fullPath, "%s/%s.csv", path, fileWoType);
    
    TwoWayZ0SCapi.setMustWork(&gp64P->twoWayP->twoWayStatusP);
    
    fdT myfd = open(fullPath, O_RDONLY);
    if(myfd < 0)
    {
        TwoWayZ0SCapi.noteFailure(&gp64P->twoWayP->twoWayStatusP);
    }
    else
    {
        int maxReadLen = FHXR_BUF_SZ - FHXR_HDSTART;
        ssize_t rdlen = read(myfd, ctrlP->buf + FHXR_HDSTART, maxReadLen);
        if(rdlen == maxReadLen)
        {
            TwoWayZ0SCapi.noteFailure(&gp64P->twoWayP->twoWayStatusP);
        }
        else
        {
            ctrlP->buf[rdlen + FHXR_HDSTART] = '\0';
        }
        
        close(myfd);
    }

    TwoWayZ0SCapi.noteSuccess(&gp64P->twoWayP->twoWayStatusP);
}
/**
 * Convert the field (column) counter to a DNV index then store the data pointer there.
 * Programming note: this function can detect UCI_FULLNM if that works for the import
 * as a time to clear all non-cloning non- guaranteed values.
 */
static void
FHO0_setColVal(int colCtr, char *beg, char *end)
{
    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[colToDnv[colCtr]];
    
    if(dnvP->uci == UCI_FULLNM){ // or some universally better beginning point
        // possibly clear optional values.
    }
    
    if(*beg == '"')
    {
        beg++;
        end--;
    }
    
    dnvP->value = beg;
    dnvP->length = (int)(end - beg);
}

/**
 * A debugging tool.
 */
static void
FHO0_describe(lineNbrT lineNbr, int colCtr, int rowCtr, int fileCtr)
{
    int dnvCtr = colToDnv[colCtr]; // redirect csv to dictionaryAndValue index
    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
    
    printf("%4i [%3d-%4d] %-18s '%.*s'\n",
           lineNbr,
           fileCtr,
           rowCtr,
           dnvP->list[0], // the default column name for this dnv.
           (int)dnvP->length,
           dnvP->value);
}

/**
 * Provides for the row the category, source, index, and a why of ACTIVE_ROWST
 * along with the row separator.
 * The What and indeX were placed in the output by the caller.
 */
static void
FHO0_getCatWhYwhoZ(char *here)
{
    FHZ0bufControlACdataPT ctrlP = &FHZ0control;
    
    sprintf(here,
            "=v" FHA_CATEGORY "=z%s=y%c" RSS_ARC,
            ctrlP->fileName,
            ACTIVE_ROWST);
}

/**
 * Put the record if it is the right size and did not cross csv import.
 * This can fail from record too large or crossing over not yet processed csv import.
 * Duplicates will be dropped and counted.
 */
static void
FHO0_checkThenPutInfo(int line, char *record, char *from, gpSllgChar64PT gp64P)
{
    FHZ0bufControlACdataPT ctrlP = &FHZ0control;
    if(gp64P->twoWayP->twoWayStatusP != KNOW_NO_ARC)
    {
        TwoWayZ0SCapi.setMustWork(&gp64P->twoWayP->twoWayStatusP);
        
        if(strlen(record) < FHXR_OUTSZ)
        {
            //printf("%4i '%s'\n", line, record);
            
            char *dupChkP = ctrlP->buf;
            while(dupChkP < ctrlP->currWrite)
            {
                if(strncmp(dupChkP, record, strlen(record)) == 0)
                {
                    // Don't store duplicates
                    // Programming note: because this uses strncmp(a, b, strlen of the incoming record)
                    // the system must be designed to avoid false duplicate detection.
                    // TODO: distinguish between batchId records and other records
                    ctrlP->droppedCount++;
                    TwoWayZ0SCapi.setNull(&gp64P->twoWayP->twoWayStatusP); // KNOW_NULL_ARC
                    break;
                }
                
                dupChkP += strlen(dupChkP) + 1;
            }
            
            if(gp64P->twoWayP->twoWayStatusP != KNOW_NULL_ARC)
            {
                strcpy(ctrlP->currWrite, record);
                ctrlP->currWrite += strlen(ctrlP->currWrite);
                ctrlP->addedCount++;
                
                FHO0_getCatWhYwhoZ(ctrlP->currWrite);
                
                // Advance pointer and check for overlap.
                ctrlP->currWrite += strlen(ctrlP->currWrite) + 1;
                if(ctrlP->currWrite >= from)
                {
                    TwoWayZ0SCapi.noteFailure(&gp64P->twoWayP->twoWayStatusP);
                }
            }
            
            // If it is KNOW_NO then the output overwrote the import else...
            if(gp64P->twoWayP->twoWayStatusP != KNOW_NO_ARC)
            {
                // ...no showstopper is seen so good (guarantee by setting must work before success).
                TwoWayZ0SCapi.setMustWork(&gp64P->twoWayP->twoWayStatusP);
                TwoWayZ0SCapi.noteSuccess(&gp64P->twoWayP->twoWayStatusP);
            }
        }
        else{
            TwoWayZ0SCapi.noteFailure(&gp64P->twoWayP->twoWayStatusP);
        }
    }
}

/**
 * Replace a DNV value with a Pool value.
 */
static char*
FHO0_strPoolToDnvCol(char **poolPP, universalColumnIdGenT uciCtr, int lenCol)
{
    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[uciToDnv[uciCtr]];
    char wk[10];
    int lenWk = sprintf(wk, "=%c", dnvP->control[FHA_LTR_IN_ROW]);
    
    char* foundAt = strstr(*poolPP, wk);
    if(foundAt){
        foundAt += lenWk;
        dnvP->value = foundAt;
        dnvP->length = lenCol;
    }
    
    return foundAt;
}

/**
 * Locate a value within a pool and position on the = of the next field.
 */
static char*
FHO0_strColDataInPool(char **poolPP, universalColumnIdGenT uciCtr)
{
    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[uciToDnv[uciCtr]];
    char wk[1000];
    int len = sprintf(wk, "=%c%.*s=", dnvP->control[FHA_LTR_IN_ROW], (int)dnvP->length, dnvP->value);
    
    char* foundAt = strstr(*poolPP, wk);
    if(foundAt){
        foundAt += (len - 1);
    }
    
    return foundAt;
}

/**
 * Check for this value in this column in this row.
 */
static bool
FHO0_someColIs(char* check, universalColumnIdGenT uciId)
{
    bool retVal = false;
    
    int dnvCtr = uciToDnv[uciId]; // redirect columnIdUniversal to dictionaryAndValue index
    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
    
    if(strncmp(dnvP->value, check, dnvP->length) == 0){
        retVal = true;
    }
    
    return retVal;
}

/**
* Use the genealogical data type (UCI) to strncatOneColumn value
* from the DNV table to where the pointer points.
* BatchId, UniversalDate, and eventType are guaranteed.
*/
static int // length or -1
FHO0_strncatOneCol(char **outPP, universalColumnIdGenT uciCtr, Ullg fieldTrkrCpy)
{
    int retVal = -1;
    
    int dnvCtr = uciToDnv[uciCtr]; // redirect columnIdUniversal to dictionaryAndValue index
    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
    // batchId always goes out if requested
    if(uciCtr == FHA_COLID_BCHID){
        if(dnvP->length == 0){
            dnvP->value = "noBatchId";
            dnvP->length = strlen(dnvP->value);
        }
        fieldTrkrCpy = 1 << uciCtr;
    }
    // universal date always goes out if requested
    if(uciCtr == UCI_UVSLDT){
        if(dnvP->length == 0){
            dnvP->value = "CCYYMMDD";
            dnvP->length = strlen(dnvP->value);
        }
        fieldTrkrCpy = 1 << uciCtr;
    }
    // eventType always goes out if requested
    if(uciCtr == UCI_EVTTP){
        if(dnvP->length == 0){
            dnvP->value = "?";
            dnvP->length = strlen(dnvP->value);
        }
        fieldTrkrCpy = 1 << uciCtr;
    }
    
    *outPP += strlen(*outPP);
    if(fieldTrkrCpy & (1 << uciCtr))
    {
        sprintf(*outPP,
                "=%c%.*s",
                dnvP->control[FHA_LTR_IN_ROW], // field letter
                (int)dnvP->length,
                dnvP->value);
        *outPP += strlen(*outPP);
        retVal = (int)dnvP->length;
    }
    
    return retVal;
}// END FHO0_strncatOneCol


/**
 * Use the genealogical data type (UCI) to export the column value from the DNV table.
 * BatchId, UniversalDate, and eventType are guaranteed.
 * To clarify this functionality, the name strncatOneCol() is available to replace it above.
 * As work is done with each calling function then strncatOneCol() will replace this name.
 */
static int // the length or -1
FHO0_exportOneCol(char **outPP, universalColumnIdGenT uniCol, Ullg fieldTrkrCpy)
{
//    int retVal = -1;
//
//    int dnvCtr = uciToDnv[uciCtr]; // redirect columnIdUniversal to dictionaryAndValue index
//    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
//    // batchId always goes out if requested
//    if(uciCtr == FHA_COLID_BCHID){
//        if(dnvP->length == 0){
//            dnvP->value = "noBatchId";
//            dnvP->length = strlen(dnvP->value);
//        }
//        fieldTrkrCpy = 1 << uciCtr;
//    }
//    // universal date always goes out if requested
//    if(uciCtr == UCI_UVSLDT){
//        if(dnvP->length == 0){
//            dnvP->value = "CCYYMMDD";
//            dnvP->length = strlen(dnvP->value);
//        }
//        fieldTrkrCpy = 1 << uciCtr;
//    }
//    // eventType always goes out if requested
//    if(uciCtr == UCI_EVTTP){
//        if(dnvP->length == 0){
//            dnvP->value = "?";
//            dnvP->length = strlen(dnvP->value);
//        }
//        fieldTrkrCpy = 1 << uciCtr;
//    }
//
//    if(fieldTrkrCpy & (1 << uciCtr))
//    {
//        sprintf(*outPP,
//                "=%c%.*s",
//                dnvP->control[FHA_LTR_IN_ROW], // field letter
//                (int)dnvP->length,
//                dnvP->value);
//        *outP += strlen(*outP);
//        retVal = (int)dnvP->length;
//    }
    
    return FHO0_strncatOneCol(outPP, uniCol, fieldTrkrCpy);
}


//Meta
/**
 * Meta data
 * - argument named from is the buffer address where the next csv row is to detect overlap
 */
static void
FHO0_meta(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UCI_ROLEINREC) ||
       fieldTrkr & (1 << UCI_RELTOHEAD) ||
       fieldTrkr & (1 << FHA_COLID_PVDDID))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;

        strcpy(outP, "=wFHUMeta");
        outP += strlen(outP);

        FHO0_strncatOneCol(&outP, UCI_ROLEINREC,    fieldTrkr);
        FHO0_strncatOneCol(&outP, UCI_RELTOHEAD,    fieldTrkr);
        FHO0_strncatOneCol(&outP, FHA_COLID_PVDDID,        fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC){
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END Meta
}

/**
 * Passed in controls
 */
static void
FHO0_format1(Ullg fieldTrkr, char* from,
             char* what, universalColumnIdGenT uciDtCol,  universalColumnIdGenT uciPlCol,
             gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << uciDtCol) || fieldTrkr & (1 << uciPlCol))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        strcat(outP, "=w");
        strcat(outP, what);

//        if(fieldTrkr & (1 << UCI_CDT) || fieldTrkr & (1 << UCI_CPLC) ||
//           fieldTrkr & (1 << UCI_MDT) || fieldTrkr & (1 << UCI_MPLC) ||
//           fieldTrkr & (1 << UCI_DDT) || fieldTrkr & (1 << UCI_DPLC) ||
//           fieldTrkr & (1 << UCI_IDT) || fieldTrkr & (1 << UCI_IPLC))
//        {
//            strcpy(outP, "=wFHX2B");
//        }
//        else
//        {
//            strcpy(outP, "=wFHBir");
//        }
//        outP += strlen(outP);
        
        FHO0_strncatOneCol(&outP, UCI_FULLNM,   fieldTrkr);
        FHO0_strncatOneCol(&outP, UCI_GNDR,     fieldTrkr);
        FHO0_strncatOneCol(&outP, uciDtCol,     fieldTrkr);
        FHO0_strncatOneCol(&outP, uciPlCol,     fieldTrkr);
        FHO0_strncatOneCol(&outP, UCI_FFNM,     fieldTrkr);
        FHO0_strncatOneCol(&outP, UCI_MFNM,     fieldTrkr);
        FHO0_strncatOneCol(&outP, FHA_COLID_PVDDID, fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC){
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END format1
}

/**
 * Birth (from birth record or other source X)
 */
static void
FHO0_birth1(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    FHO0_format1(fieldTrkr, from, "FHBir", UCI_BDT, UCI_BPLC, gp64P);
//    if(fieldTrkr & (1 << UCI_BDT) || fieldTrkr & (1 << UCI_BPLC))
//    {
//        char record[FHXR_OUTSZ] = "";
//        char *outP = record;
//
//        if(fieldTrkr & (1 << UCI_CDT) || fieldTrkr & (1 << UCI_CPLC) ||
//           fieldTrkr & (1 << UCI_MDT) || fieldTrkr & (1 << UCI_MPLC) ||
//           fieldTrkr & (1 << UCI_DDT) || fieldTrkr & (1 << UCI_DPLC) ||
//           fieldTrkr & (1 << UCI_IDT) || fieldTrkr & (1 << UCI_IPLC))
//        {
//            strcpy(outP, "=wFHX2B");
//        }
//        else
//        {
//            strcpy(outP, "=wFHBir");
//        }
//        outP += strlen(outP);
//        
//        FHO0_strncatOneCol(&outP, UCI_FULLNM,   fieldTrkr);
//        FHO0_strncatOneCol(&outP, UCI_GNDR,     fieldTrkr);
//        FHO0_strncatOneCol(&outP, UCI_BDT,      fieldTrkr);
//        FHO0_strncatOneCol(&outP, UCI_BPLC,     fieldTrkr);
//        FHO0_strncatOneCol(&outP, UCI_FFNM,     fieldTrkr);
//        FHO0_strncatOneCol(&outP, UCI_MFNM,     fieldTrkr);
//        FHO0_strncatOneCol(&outP, FHA_COLID_PVDDID,    fieldTrkr);
//        
//        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
//        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC){
//            FHZ0control.linePresentingError = __LINE__;
//        }
//    }//END Birthday
}

// Christening
static void
FHO0_chris(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UCI_CDT) || fieldTrkr & (1 << UCI_CPLC))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(outP, "=wFHChr");
        outP += strlen(outP);
        FHO0_exportOneCol(&outP, UCI_FULLNM,  fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_CDT,     fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_CPLC,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_FFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_MFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, FHA_COLID_PVDDID,   fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END Christening
}



// Marriage
static void
FHO0_marry(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UCI_MDT) || fieldTrkr & (1 << UCI_MPLC) ||
       fieldTrkr & (1 << UCI_SFNM))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(outP, "=wFHMar");
        outP += strlen(outP);
        FHO0_exportOneCol(&outP, UCI_FULLNM,  fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_MDT,     fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_MPLC,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_FFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_MFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_SFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, FHA_COLID_PVDDID,   fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END Marriage
}

// Death (from birth record or other source X)
static void
FHO0_death(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UCI_DDT) || fieldTrkr & (1 << UCI_DPLC))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        if(fieldTrkr & (1 << UCI_BDT) || fieldTrkr & (1 << UCI_BPLC) ||
           fieldTrkr & (1 << UCI_CDT) || fieldTrkr & (1 << UCI_CPLC) ||
           fieldTrkr & (1 << UCI_MDT) || fieldTrkr & (1 << UCI_MPLC) ||
           fieldTrkr & (1 << UCI_IDT) || fieldTrkr & (1 << UCI_IPLC))
        {
            strcpy(outP, "=wFHX2D"); // some other record also has death date
        }
        else
        {
            strcpy(outP, "=wFHDea");
        }
        outP += strlen(outP);
        
        FHO0_exportOneCol(&outP, UCI_FULLNM,  fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_GNDR,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_DDT,     fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_DPLC,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_FFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_MFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_SFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, FHA_COLID_PVDDID,   fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END Death
}

// Burial/Internment
static void
FHO0_bury(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UCI_IDT) || fieldTrkr & (1 << UCI_IPLC))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(outP, "=wFHIntern");
        outP += strlen(outP);
        
        FHO0_exportOneCol(&outP, UCI_FULLNM,  fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_IDT,     fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_IPLC,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_FFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_MFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, UCI_SFNM,    fieldTrkr);
        FHO0_exportOneCol(&outP, FHA_COLID_PVDDID,   fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END Burial/Internment
}

/**
 * children, other, parents full names and other events.
 */
static void
FHO0_other(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UCI_CFNMS) ||
       fieldTrkr & (1 << UCI_OFNMS) ||
       fieldTrkr & (1 << UCI_PFNMS) ||
       fieldTrkr & (1 << UCI_OEVENTS))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(outP, "=wFHOth");
        outP += strlen(outP);
        FHO0_strncatOneCol(&outP, UCI_CFNMS,   fieldTrkr);
        FHO0_strncatOneCol(&outP, UCI_OFNMS,   fieldTrkr);
        FHO0_strncatOneCol(&outP, UCI_PFNMS,   fieldTrkr);
        FHO0_strncatOneCol(&outP, UCI_OEVENTS, fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END Other
}

/**
 * Export the unique batch numbers.
 */
static void
FHO0_batchId(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << FHA_COLID_BCHID))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(outP, "=w" FHSEL_BATCHID);
        outP += strlen(outP);
        FHO0_strncatOneCol(&outP, FHA_COLID_BCHID,   fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END BatchIx
}

static void
FHO0_abuseResDt(char** outPP, char* toMakeNow)
{
    Ulng fieldTrkr = (1<<UCI_RESDTB);
    int dnvCtr = uciToDnv[UCI_RESDTB]; // redirect columnIdUniversal to dictionaryAndValue index
    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
    dnvP->value = toMakeNow;
    dnvP->length = strlen(dnvP->value);
    FHO0_strncatOneCol(outPP, UCI_RESDTB, fieldTrkr);
}

/**
 * Pick from the first normal form data the place associated with the event type.
 * Programming note: UCI_EVTTP must be ready if calculated.
 */
static void
FHO0_eventPlaceForType(char** outPP, Ullg fieldTrkrCpy, char* from)
{
    int dnvCtr = uciToDnv[UCI_EVTTP]; // redirect columnIdUniversal to dictionaryAndValue index
    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];

    if(fieldTrkrCpy & (1 << UCI_BPLC) && dnvP->value[0] == UCI_EVTTP_BIR[0]){
        FHO0_exportOneCol(outPP, UCI_BPLC,   fieldTrkrCpy);
    }else if(fieldTrkrCpy & (1 << UCI_MPLC) && dnvP->value[0] == UCI_EVTTP_MAR[0]){
        FHO0_exportOneCol(outPP, UCI_MPLC,   fieldTrkrCpy);
    }else if(fieldTrkrCpy & (1 << UCI_DPLC) && dnvP->value[0] == UCI_EVTTP_DTH[0]){
        FHO0_exportOneCol(outPP, UCI_DPLC,   fieldTrkrCpy);
    }else if(fieldTrkrCpy & (1 << UCI_IPLC) && dnvP->value[0] == UCI_EVTTP_BRY[0]){
        FHO0_exportOneCol(outPP, UCI_IPLC,   fieldTrkrCpy);
    }else if(fieldTrkrCpy & (1 << UCI_CPLC) && dnvP->value[0] == UCI_EVTTP_CHR[0]){
        FHO0_exportOneCol(outPP, UCI_CPLC,   fieldTrkrCpy);
    }else if(fieldTrkrCpy & (1 << UCI_RESPLC) && dnvP->value[0] == UCI_EVTTP_RES[0]){
        FHO0_exportOneCol(outPP, UCI_RESPLC,   fieldTrkrCpy);
    }else{
        int dnvCtr = uciToDnv[UCI_RESPLC]; // redirect columnIdUniversal to dictionaryAndValue index
        FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
        fieldTrkrCpy |= (1<<UCI_RESPLC); // local
        dnvP->value = "noPlace";
        dnvP->length = strlen(dnvP->value);
        FHO0_exportOneCol(outPP, UCI_RESPLC, fieldTrkrCpy);
    }
}

/**
 * From the various dates, create the event type as needed.
 * Programming note: when eventType depending on the source is calculated
 * then it will not be available until all the dependancy imports are processed.
 */
static void
 FHO0_calcType(char* calcTypeP)
 {
     int dnvCtr = uciToDnv[UCI_EVTTP]; // redirect columnIdUniversal to dictionaryAndValue index
     FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
     
     dnvP->value = calcTypeP;
     dnvP->length = strlen(dnvP->value);
 }

/**
 * Obsolete this (used only by FHO0_nmDtBatchId) in favor of nthDate.
 */
static Ulng
FHO0_bestDate(char** outPP, Ullg fieldTrkr, char* from)
{
    char* keepOutP = *outPP;
    
    if(fieldTrkr & (1 << UCI_BDT)){
        FHO0_calcType(UCI_EVTTP_BIR);
        FHO0_exportOneCol(outPP, UCI_BDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_MDT)){
        FHO0_calcType(UCI_EVTTP_MAR);
        FHO0_exportOneCol(outPP, UCI_MDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_DDT)){
        FHO0_calcType(UCI_EVTTP_DTH);
        FHO0_exportOneCol(outPP, UCI_DDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_IDT)){
        FHO0_calcType(UCI_EVTTP_BRY);
        FHO0_exportOneCol(outPP, UCI_IDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_CDT)){
        FHO0_calcType(UCI_EVTTP_CHR);
        FHO0_exportOneCol(outPP, UCI_CDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_RESDTB)){
        FHO0_calcType(UCI_EVTTP_RES);
        FHO0_exportOneCol(outPP, UCI_RESDTB,   fieldTrkr);
    }else{
        FHO0_calcType(UCI_EVTTP_RES);
        FHO0_abuseResDt(outPP, "00 Unk 0000");
    }
    
    keepOutP += 2;
    Ulng bestDate = strtol(keepOutP, NO_ARG_PTR_ARC, RADIX_10_ARC);
    if(bestDate > 1000){
        bestDate *= 100 * 100; // Only a year is given so finish it
    }else if(bestDate <= 31){
        // this is a day of the month so keep it and move to the alpha month
        bestDate += DictO0SCapi.getOfAlphaMon(keepOutP) * 100;
        bestDate += strtol(*outPP - 4, NO_ARG_PTR_ARC, RADIX_10_ARC) * 100 * 100;
    }
    
    return bestDate;
}

/**
 * Given multiple dates and places in a first normal form record
 * strncat the associated event type.
 */
static Ulng
FHO0_nthDate(char** outPP, int dateNbr, Ullg fieldTrkr, char* from)
{
    char* keepOutP = *outPP;
    
    if(fieldTrkr & (1 << UCI_BDT) && (--dateNbr == 0)){
        FHO0_calcType(UCI_EVTTP_BIR);
        FHO0_exportOneCol(outPP, UCI_BDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_MDT) && (--dateNbr == 0)){
        FHO0_calcType(UCI_EVTTP_MAR);
        FHO0_exportOneCol(outPP, UCI_MDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_DDT) && (--dateNbr == 0)){
        FHO0_calcType(UCI_EVTTP_DTH);
        FHO0_exportOneCol(outPP, UCI_DDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_IDT) && (--dateNbr == 0)){
        FHO0_calcType(UCI_EVTTP_BRY);
        FHO0_exportOneCol(outPP, UCI_IDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_CDT) && (--dateNbr == 0)){
        FHO0_calcType(UCI_EVTTP_CHR);
        FHO0_exportOneCol(outPP, UCI_CDT,   fieldTrkr);
    }else /*if(fieldTrkr & (1 << UCI_RESDTB) && (--dateNbr == 0))*/{
        FHO0_calcType(UCI_EVTTP_RES);
        FHO0_exportOneCol(outPP, UCI_RESDTB,   fieldTrkr);
    }
    
    keepOutP += 2;
    Ulng nthDate = strtol(keepOutP, NO_ARG_PTR_ARC, RADIX_10_ARC);
    if(nthDate > 1000){
        nthDate *= 100 * 100; // Only a year is given so finish it
    }else if(nthDate <= 31){
        // this is a day of the month so keep it and move to the alpha month
        nthDate += DictO0SCapi.getOfAlphaMon(keepOutP) * 100;
        nthDate += strtol(*outPP - 4, NO_ARG_PTR_ARC, RADIX_10_ARC) * 100 * 100;
    }
    
    return nthDate;
}
/**
 * Export the unique batch number and place for each event.
 */
static void
FHO0_batchIdPlace(int dateNbr, Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(dateNbr > 0 &&
       fieldTrkr & (1 << FHA_COLID_BCHID))
    {
        char  record[FHXR_OUTSZ] = "";
        char* outP = record;
        
        char  dateWork[FHXR_OUTSZ] = ""; // a place to waste the unwanted export
        char* dateWorkP = dateWork;
        
        strcpy(outP, "=w" FHSEL_BCHIDPLC);
        outP += strlen(outP);
        FHO0_strncatOneCol(&outP, FHA_COLID_BCHID,   fieldTrkr);
        FHO0_nthDate(&dateWorkP, dateNbr, fieldTrkr, from); // returns best CCYYMMDD;
        FHO0_eventPlaceForType(&outP, fieldTrkr, from);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END BatchIdPlace
}

/**
 * Export the score, fullName, bestDate, eventPlace, pvddId, BatchId
 * TODO: not sure why PVDDID is in here.
 */
static void
FHO0_nmDtBatchId(Ullg fieldTrkrCpy, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkrCpy & (1 << FHA_COLID_BCHID) ||
       fieldTrkrCpy & (1 << FHA_COLID_PVDDID)  ||
       fieldTrkrCpy & (1 << UCI_FULLNM) ||
       fieldTrkrCpy & (1 << UCI_BDT) ||
       fieldTrkrCpy & (1 << UCI_CDT) ||
       fieldTrkrCpy & (1 << UCI_MDT) ||
       fieldTrkrCpy & (1 << UCI_DDT) ||
       fieldTrkrCpy & (1 << UCI_IDT))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(outP, "=wFHNmDtBch");
        outP += strlen(outP);
        FHO0_strncatOneCol(&outP, FHA_COLID_SCORE,   fieldTrkrCpy);
        FHO0_strncatOneCol(&outP, UCI_FULLNM,  fieldTrkrCpy);
        FHO0_bestDate(&outP, fieldTrkrCpy, from); // also returns CCYYMMDD
        FHO0_eventPlaceForType(&outP, fieldTrkrCpy, from);
        
        FHO0_strncatOneCol(&outP, FHA_COLID_PVDDID,  fieldTrkrCpy);
        FHO0_strncatOneCol(&outP, FHA_COLID_BCHID,  fieldTrkrCpy);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END NameDateBatchId
}
/**
 * Gather all the fullName, eventDate, BatchId,
 * score, familyDate, date, primaryId,  batchId, fullName, otherFullNames.
 * The dateNbr may be internally modified so is marked clearly as a copy.
 */
static void
FHO0_seekFind(int dateNbrCpy, Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    // programable breakpoints for any data type
    bool pauseHere = false;
    if(FHO0_someColIs("ark:/61903/1:1:VH65-6RJ", FHA_COLID_PVDDID)){
        pauseHere = true;
    }
    if(dateNbrCpy == 2){
        pauseHere = true;
    }
    // The Adjustments may contain a delete for this PVDDID
    // in which case it must be ignored by turning off selection.
    // See also newestUniversalDate below.
    // void checkDeselect()
    {
        char chkDeselection[FHXR_OUTSZ] = "";
        
        char* chkDeselectionP = chkDeselection;
        FHO0_strncatOneCol(&chkDeselectionP, FHA_COLID_PVDDID, fieldTrkr);
        if(strstr(FHZ0duplicatedIds, chkDeselection+2)){
            dateNbrCpy = 0;
        }
    }
    
    if(dateNbrCpy > 0 &&
       fieldTrkr & (1 << FHA_COLID_BCHID) &&
       fieldTrkr & (1 << UCI_FULLNM) &&
       fieldTrkr & (1 << FHA_COLID_PVDDID))
    {
        // Get that precious event type, date, and place
        char  familyDate[FHXR_OUTSZ] = "";
        char* familyDateP = familyDate;
        
        long eventDate = FHO0_nthDate(&familyDateP, dateNbrCpy, fieldTrkr, from); // returns numbered CCYYMMDD;
        // this becomes a placeholder
        FHO0_strncatOneCol(&familyDateP, UCI_UVSLDT, fieldTrkr | (1<<UCI_UVSLDT));
        
        // If the record is too new then ignore it.
        if(eventDate > newestUniversalDate ||
           eventDate < oldestUniversalDate){
            eventDate = 0;
        }
        
        if(eventDate != 0)
        {
            char common[FHXR_OUTSZ] = "";
            char* commonP = common;
            char workNames[FHXR_OUTSZ] = "";
            char* isFamilyRevHereInPoolP = 0;
            
            // void createReusableData()
            {
                // void gather() all the available names TODO: other_full_names?
                {
                    char* workNamesP = workNames;
                    FHO0_strncatOneCol(&workNamesP, UCI_FULLNM, fieldTrkr);
                    FHO0_strncatOneCol(&workNamesP, UCI_FFNM,   fieldTrkr);
                    FHO0_strncatOneCol(&workNamesP, UCI_MFNM,   fieldTrkr);
                    FHO0_strncatOneCol(&workNamesP, UCI_SFNM,   fieldTrkr);
                    FHO0_strncatOneCol(&workNamesP, UCI_PFNMS,  fieldTrkr);
                }
                
                // These records focus on the principle individual of the recorded event.
                // Place is not used because people can travel great distances in a year.
                // void gather() the providedId, batchId, constant score, and event type for reuse
                {
                    FHO0_strncatOneCol(&commonP, FHA_COLID_PVDDID,   fieldTrkr);
                    FHO0_strncatOneCol(&commonP, FHA_COLID_BCHID,    fieldTrkr);
                    
                    char* familyP = FHZ0familyIds;
                    isFamilyRevHereInPoolP = FHO0_strColDataInPool(&familyP, FHA_COLID_PVDDID);
                    if(isFamilyRevHereInPoolP)
                    {
                        // Score comes from the family revision.
                        char* cpyP = isFamilyRevHereInPoolP;
                        FHO0_strPoolToDnvCol(&cpyP, FHA_COLID_SCORE, 6);
                    }else{
                        // Score must be zeroed due to rows coming from various exports
                        // and the varying score making a nonunique row appear unique.
                        FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[uciToDnv[FHA_COLID_SCORE]];
                        dnvP->value = "0";
                        dnvP->length = strlen(dnvP->value);
                    }
                    FHO0_strncatOneCol(&commonP, FHA_COLID_SCORE, fieldTrkr);
                }
                // Back to basics.
                FHO0_strncatOneCol(&commonP, UCI_EVTTP, fieldTrkr);
                strcat(commonP, familyDate);
                commonP += strlen(commonP);
            }
            // The end of setting up.
            // Now make the four people depending on what data is available.
            // which is the what, the name, and all common information.
            // The slightly varied UCI_UVSLDT overlays then will be replaced by family modification.
            
            // void makePrinciplePerson()
            {
                char person[FHXR_OUTSZ] = "";
                char* personP = person;
                strcpy(personP, "=w" FHSEL_FINDME);
                strcat(personP, common);
                personP += strlen(personP);
                // Here come the two dates...
                // then overlaying the original event date
                // then adding the names.
                sprintf(personP - 8, "%08li", eventDate);
                strcat(personP, workNames);
                FHO0_checkThenPutInfo(__LINE__, person, from, gp64P);
            }
            
            if(dateNbrCpy == 1 ||
               ! FHO0_someColIs(UCI_EVTTP_RES, UCI_EVTTP))
            {
                if(isFamilyRevHereInPoolP)
                {
                    char* cpyP = isFamilyRevHereInPoolP;
                    char* foundAtCpy = FHO0_strPoolToDnvCol(&cpyP, UCI_UVSLDT, 8);
                    strncpy(commonP-8, foundAtCpy, 8);
                }else{
                    sprintf(commonP-8, "%08li", eventDate - 0);
                }
                
                // void generateFather() version of the record if possible.
                {
                    char father[FHXR_OUTSZ] = "";
                    char* fatherP = father;
                    if(fieldTrkr & (1 << UCI_FFNM) ){
                        strcpy(fatherP, "=w" FHSEL_SEEKPA);
                        strcat(fatherP, common);
                        strcat(fatherP, workNames);
                        FHO0_checkThenPutInfo(__LINE__, father, from, gp64P);
                    }
                }
                
                // void generateMother() version of the record if possible.
                {
                    char mother[FHXR_OUTSZ] = "";
                    char* motherP = mother;
                    if(fieldTrkr & (1 << UCI_MFNM) ){
                        strcpy(motherP, "=w" FHSEL_SEEKMA);
                        strcat(motherP, common);
                        strcat(motherP, workNames);
                        FHO0_checkThenPutInfo(__LINE__, mother, from, gp64P);
                    }
                }
                
                // void generateSpouse() version of the record if possible.
                {
                    char spouse[FHXR_OUTSZ] = "";
                    char* spouseP = spouse;
                    if(fieldTrkr & (1 << UCI_SFNM) ){
                        strcpy(spouseP, "=w" FHSEL_SEEKSP);
                        strcat(spouseP, common);
                        strcat(spouseP, workNames);
                        FHO0_checkThenPutInfo(__LINE__, spouse, from, gp64P);
                    }
                }
            }// END if not residence as only event in record.
        }// END if eventDate exists or remains
    }// END ifIhaveEnoughDatesAndInfo
}

/**
 * Load an adjustment:
 * - a person_id that has duplicated information (event_type == D)
 */
static void
FHO0_Adjustment(Ullg fieldTrkr, gpSllgChar64PT gp64P)
{
    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[uciToDnv[UCI_EVTTP]];
    if(strncmp(UCI_EVTTP_DEL, dnvP->value, dnvP->length) == 0)
    {
        // the type
        FHO0_strncatOneCol(&FHZ0duplicatedIdsP, UCI_EVTTP, fieldTrkr);
        // the pvddId (provided Id)
        FHO0_strncatOneCol(&FHZ0duplicatedIdsP, FHA_COLID_PVDDID, fieldTrkr);
    }else if(strncmp(UCI_EVTTP_FAM, dnvP->value, dnvP->length) == 0)
    {
        // the type
        FHO0_strncatOneCol(&FHZ0familyIdsP, UCI_EVTTP, fieldTrkr);
        FHO0_strncatOneCol(&FHZ0familyIdsP, FHA_COLID_PVDDID, fieldTrkr);
        FHO0_strncatOneCol(&FHZ0familyIdsP, FHA_COLID_SCORE, fieldTrkr);
        FHO0_strncatOneCol(&FHZ0familyIdsP, UCI_UVSLDT, fieldTrkr);
    }

    FHZ0bufControlACdataPT ctrlP = &FHZ0control;
    ctrlP->addedCount++;
}

/**
 * Load up the buffers with a file,
 */
static void
FHO0_newFile(char* path, fileWoTypeT file, FHZ0SelectionT selId, gpSllgChar64PT gp64P)
{
    TwoWayZ0SCapi.setMustWork(&gp64P->twoWayP->twoWayStatusP);
    
    if(FHZ0control.currentRead == 0)
    {
        FHZ0control.currWrite = FHZ0control.currentRead = FHZ0control.buf;
        FHZ0control.fileNbr = 0;
    }
    FHZ0control.linePresentingError = 0;
    FHZ0control.droppedCount = 0;
    FHZ0control.addedCount = 0;
    FHZ0control.fileNbr++;
    
    FHO0_OpenReadClose(path, file, gp64P);
    
    if(gp64P->twoWayP->twoWayStatusP == KNOW_YES_ARC)
    {
        char *tokenNxtP = strstr(FHZ0control.buf + FHXR_HDSTART, FHXRCH_BEGIN); // TODO: depends on file source
        
        int   doAtLeast = 6;
        int   maxShift = 6;
        int   condense = HB_CONDENSE;
        int   byColumn = HB_COL_NBR_BASE;
        char *fieldSeps = ",";
        char *rowSeps = "\n\r";
        int   hashMod = HB_BIG_HASH_RSVD;
        
        HashBasicsCtlT colHdrHashCtl =
        {
            doAtLeast,
            maxShift,
            condense,
            byColumn,
            hashMod,
            fieldSeps,
            rowSeps,
            tokenNxtP
        };
        
        
        FHZ0control.rowNbr = 0;
        FHZ0control.colNbr = 0;
        unsigned long long fieldTrkr = 0;
        int doExtraTimeIn = 1; // to also get the output after the final import
        colHdrHashCtl.tokenEndP = colHdrHashCtl.tokenNxtP; // prevent null address.
        while(*colHdrHashCtl.tokenNxtP || doExtraTimeIn)
        {
            if(*(colHdrHashCtl.tokenEndP+0) == 0 ||
               strchr(rowSeps, *(colHdrHashCtl.tokenEndP+1)) != 0)
            {
                // We are at a row separator.
                if(FHZ0control.rowNbr > 0){
                    // Programming note: fieldTrkr is 0 on the header,
                    // but coding the ifRowNbr above feels safer and reduces work.
                    int dateCnt = 0;
                    if(fieldTrkr & (1<<UCI_RESDTB)){dateCnt++;}
                    if(fieldTrkr & (1<<UCI_BDT)){dateCnt++;}
                    if(fieldTrkr & (1<<UCI_CDT)){dateCnt++;}
                    if(fieldTrkr & (1<<UCI_MDT)){dateCnt++;}
                    if(fieldTrkr & (1<<UCI_DDT)){dateCnt++;}
                    if(fieldTrkr & (1<<UCI_IDT)){dateCnt++;}
                    
                    if(dateCnt <= 1){
                        if(dateCnt == 0 &&
                           fieldTrkr & (1<<UCI_EVTTP) &&
                           fieldTrkr & (1<<UCI_UVSLDT) &&
                           fieldTrkr & (1<<FHA_COLID_SCORE) &&
                           fieldTrkr & (1<<FHA_COLID_PVDDID))
                        {
                            // This is an adjustment.
                            FHO0_Adjustment(fieldTrkr, gp64P);
                        }else{
                            switch(selId)
                            {
                                case FHZ0_SelBatchId:
                                    FHO0_batchId(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelBatchIdPlace:
                                    FHO0_batchIdPlace(dateCnt, fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelSeekFind:
                                    FHO0_seekFind(dateCnt, fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelMetaData:
                                    FHO0_meta (fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelBirth:
                                    FHO0_birth1(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelChristening:
                                    FHO0_chris(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelMarriage:
                                    FHO0_marry(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelDeath:
                                    FHO0_death(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelBurial:
                                    FHO0_bury (fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelNmDtBatchId:
                                    FHO0_nmDtBatchId(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelOther:
                                    FHO0_other(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                            }//END switch selection
                        }//END else not adjustment
                    }else{
                        for(int dtScan = 1; dtScan <= dateCnt; dtScan++)
                        {
                            switch(selId)
                            {
                                case FHZ0_SelSeekFind:
                                    FHO0_seekFind(dtScan, fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                case FHZ0_SelBatchIdPlace:
                                    FHO0_batchIdPlace(dtScan, fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
                
                // If got here as final entry due to doExtraTimeIn
                // then quit voluntairly because the import is used up.
                if(! *colHdrHashCtl.tokenNxtP){
                    break;
                }
                
                // Manage controls and continue
                // because a row was completed.
                // TODO: could clear the DNV pointer and length.
                FHZ0control.colNbr = 0;
                FHZ0control.rowNbr++;
                fieldTrkr = 0;
            }
            
            // Back to processing a column...
            /*eightHashT hsh =*/ HashBasicsHCapi.getBasicHash(&colHdrHashCtl);
            
            // Either a column header (from row 0) is being processed...
            if(FHZ0control.rowNbr == 0)
            {
                if(FHO0_checkColName(FHZ0control.colNbr, colHdrHashCtl.tokenBegP) == -1)
                {
                    FHZ0control.linePresentingError = __LINE__;
                    break;
                }
            }
            else // ... or an import row is being processed.
            {
                if(colHdrHashCtl.tokenBegP != colHdrHashCtl.tokenEndP){
                    fieldTrkr += 1 << FHZ0DictionaryAndValue[colToDnv[FHZ0control.colNbr]].uci;
                }
                
                FHO0_setColVal(FHZ0control.colNbr, colHdrHashCtl.tokenBegP, colHdrHashCtl.tokenEndP);
                // Programming note: To watch a particular row,
                // put the desired file and row number in place of the 100s.
                if(FHZ0control.fileNbr >= 100 && FHZ0control.rowNbr >= 100)
                {
                    FHO0_describe(__LINE__,
                                  FHZ0control.colNbr,
                                  FHZ0control.rowNbr,
                                  FHZ0control.fileNbr);
                }
            }
            
            FHZ0control.colNbr++;
        }//END while more CSV
    }
    else{
        FHZ0control.linePresentingError = __LINE__;
    }
}

static void
FHO0_newFiles(FHZ0SelectionT selection, gpSllgChar64PT gp64P)
{
    for(int sourceNameIx = 0; FHZ0FilesACdata[sourceNameIx].export != 0; sourceNameIx++)
    {
        if(FHZ0FilesACdata[sourceNameIx].import)
        {
            FHO0_newFile(INIT_DB_PATH,
                        FHZ0FilesACdata[sourceNameIx].export,
                        selection,
                        gp64P);
            
            FHZ0FilesACdata[sourceNameIx].drops = FHZ0control.droppedCount;
            FHZ0FilesACdata[sourceNameIx].exports = FHZ0control.addedCount;
            
            printf("%4d <%d> %5d %5d %s\n", __LINE__,
                   FHZ0control.linePresentingError,
                   FHZ0FilesACdata[sourceNameIx].exports,
                   FHZ0FilesACdata[sourceNameIx].drops,
                   FHZ0FilesACdata[sourceNameIx].export);
            
            if(FHZ0control.linePresentingError != 0)
            {
                break;
            }
        }
    }
}

FHO0ACapiT FHO0ACapi =
{
    FHO0_newFiles
};

// END FamilyHistoryZ0.c
/**
 * Load data of various types from various sources:
 * - Identified in your FHZ0FilesACdata
 * - Adjustments
 * - FamilySearch exports
 * Produce to stdout various formats
 */
