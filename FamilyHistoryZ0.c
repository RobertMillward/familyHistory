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
// application api's
#include "FamilyHistoryO0.h"

#define VALHERE 0
#define LENHERE 0

static FHZ0DictionaryAndValueT FHZ0DictionaryAndValue[] = // dnv
{
    {UCI_FULLNM,    FHA_COL_PRINM,   VALHERE, LENHERE, {"full_name",          0}},
    {UCI_GNDR,      FHA_COL_PRIGN,   VALHERE, LENHERE, {"gender",             0}},
    {UCI_BDT,       FHA_COL_PRIDT,   VALHERE, LENHERE, {"birth_date",         0}},
    {UCI_BPLC,      FHA_COL_PRIPL,   VALHERE, LENHERE, {"birth_place_text",   0}},
    {UCI_CDT,       FHA_COL_PRIDT,   VALHERE, LENHERE, {"chr_date",           0}},
    {UCI_CPLC,      FHA_COL_PRIPL,   VALHERE, LENHERE, {"chr_place_text",     0}},
    {UCI_MDT,       FHA_COL_PRIDT,   VALHERE, LENHERE, {"marriage_date",      0}},
    {UCI_MPLC,      FHA_COL_PRIPL,   VALHERE, LENHERE, {"marriage_place_text",0}},
    {UCI_DDT,       FHA_COL_PRIDT,   VALHERE, LENHERE, {"death_date",         0}},
    {UCI_DPLC,      FHA_COL_PRIPL,   VALHERE, LENHERE, {"death_place_text",   0}},
    {UCI_IDT,       FHA_COL_PRIDT,   VALHERE, LENHERE, {"burial_date",        0}},
    {UCI_IPLC,      FHA_COL_PRIPL,   VALHERE, LENHERE, {"burial_place_text",  0}},
    
    {UCI_FFNM,      FHA_COL_FTHNM,   VALHERE, LENHERE, {"father_full_name",   0}},
    {UCI_MFNM,      FHA_COL_MTHNM,   VALHERE, LENHERE, {"mother_full_name",   0}},
    {UCI_SFNM,      FHA_COL_SPONM,   VALHERE, LENHERE, {"spouse_full_name",   0}},
    {UCI_PFNMS,     FHA_COL_OTHPFNM, VALHERE, LENHERE, {"parent_full_names",  0}},
    {UCI_CFNMS,     FHA_COL_OTHCFNM, VALHERE, LENHERE, {"children_full_names",0}},
    {UCI_BCHNBR,    FHA_COL_BTNBR,   VALHERE, LENHERE, {"batch_number",       0}},
    {UCI_SCORE,     "XDv",           VALHERE, LENHERE, {FHXRCH_BEGIN,         0}},
    
    {UCI_RESDT,     FHA_COL_PRIDT,   VALHERE, LENHERE, {"residence_date",       0}},
    {UCI_RESPLC,    FHA_COL_PRIPL,   VALHERE, LENHERE, {"residence_place_text", 0}},
    {UCI_OFNMS,     "Xlv",           VALHERE, LENHERE, {"other_full_names",     0}},
    {UCI_OEVENTS,   FHA_COL_OTHEVNT, VALHERE, LENHERE, {"other_events",         0}},
    
    {UCI_PVDDID,    FHA_COL_PRVDID,   VALHERE, LENHERE, {"providedId", "person_url",         0}},
    {UCI_SMT,       FHA_COL_MTSRCTP, VALHERE, LENHERE, {"source_media_type",  0}},
    {UCI_ROLEINREC, FHA_COL_MTRECRL, VALHERE, LENHERE, {"role_in_record",     0}},
    {UCI_RELTOHEAD, FHA_COL_MTRLTHD, VALHERE, LENHERE, {"relationship_to_head", 0}},
    {UCI_SUBCLLID,  "Xlv",           VALHERE, LENHERE, {"subcollection_id",   0}},
    {UCI_EUID,      "Xlv",           VALHERE, LENHERE, {"easy_unique_id",     0}},
    {UCI_RURL,      "Xlv",           VALHERE, LENHERE, {"record_url",         0}},
    {0}// UCI is 1-based
};

FHZ0controlT FHZ0control;

/**
 * Upon locating in the above FHZ0DictionaryAndValue table the column name found in the input
 * store the index here for quick retrieval by column number.
 */
#define UCI_TODNV_Z 100
static int colToDnv[UCI_TODNV_Z];
/**
 * Upon locating in the above FHZ0DictionaryAndValue table the column name found in the input
 * store in [columnIdUniversalET value] the index so that the Ciu enums can quickly access the table.
 */
static int uciToDnv[UCI_TODNV_Z];



/**
 * This happens only upon finding a column header row in the input.
 * A return 0f -1 means the column header was not found.
 */
static int
FHO0_checkColName(int fieldCtr, char *begP)
{
    if(fieldCtr == 0){
        // initialize colToDnv, uciToDnv, and Dnv
        for(int initIx = 0; initIx < UCI_TODNV_Z; initIx++){
            uciToDnv[initIx] = UCI_NULL;
            colToDnv[initIx] = -1;
        }
    }
    
    int dnvIx = 0;
    for(; FHZ0DictionaryAndValue[dnvIx].uci != 0; dnvIx++)
    {
        for(int altNmIx = 0; FHZ0DictionaryAndValue[dnvIx].list[altNmIx] != 0; altNmIx++){
            if(strncmp(FHZ0DictionaryAndValue[dnvIx].list[altNmIx],
                       begP,
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
    FHZ0controlPT ctrlP = &FHZ0control;

    strcpy(ctrlP->fileName, fileWoType);
    ctrlP->rowNbr = 0;
    ctrlP->colNbr = 0;
    
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
 */
static void
FHO0_setColVal(int colCtr, char *beg, char *end)
{
    FHZ0DictionaryAndValuePT dataP = &FHZ0DictionaryAndValue[colToDnv[colCtr]];
    
    if(*beg == '"')
    {
        beg++;
        end--;
    }
    
    dataP->value = beg;
    dataP->length = (int)(end - beg);
}

static void
FHO0_makeOneCol(char **outP, int uciCtr, Ullg fieldTrkr)
{
    int dnvCtr = uciToDnv[uciCtr]; // redirect columnIdUniversal to dictionaryAndValue index
    FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
    
    if(uciCtr == UCI_BCHNBR){
        if(dnvP->length == 0){
            dnvP->value = "noBatchId";
            dnvP->length = strlen(dnvP->value);
        }
        fieldTrkr = 1 << uciCtr; // local copy of fieldTrkr
    }
    
    if(fieldTrkr & (1 << uciCtr))
    {
        sprintf(*outP,
                "=%c%.*s",
                dnvP->control[FHA_COL_IN_ROW], // field letter
                (int)dnvP->length,
                dnvP->value);
        *outP += strlen(*outP);
    }
}

static void
FHO0_describe(int line, int colCtr, int rowCtr)
{
    int dnvCtr = colToDnv[colCtr]; // redirect csv to dictionaryAndValue index
    FHZ0DictionaryAndValuePT dataP = &FHZ0DictionaryAndValue[dnvCtr];
    
    printf("%4i [%4d] %-18s '%.*s'\n",
       line,
       rowCtr,
       dataP->list[0],
       (int)dataP->length,
       dataP->value);
}

/**
 * Provides for the row the source, the index, and a why of ACTIVE_ROWST
 * along with the row separator.
 * The what and indeX was provided by the caller.
 */
static void
FHO0_getWhYwhoZ(char *here)
{
    FHZ0controlPT ctrlP = &FHZ0control;
    
    sprintf(here,
            "=z%s=y%c" RSS_ARC,
            ctrlP->fileName,
            ACTIVE_ROWST);
}

/**
 * Put the record if it is the right size and did not cross csv input.
 * This can fail from record too large or crossing over not yet processed csv input.
 * Duplicates will be dropped and counted.
 */
static void
FHO0_checkThenPutInfo(int line, char *record, char *from, gpSllgChar64PT gp64P)
{
    FHZ0controlPT ctrlP = &FHZ0control;
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
                // Programming note: because this uses strlen of the incoming record
                // the system must be designed to avoid false duplicate detection.
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
            
            FHO0_getWhYwhoZ(ctrlP->currWrite);
            
            // Advance pointer and check for overlap.
            ctrlP->currWrite += strlen(ctrlP->currWrite) + 1;
            if(ctrlP->currWrite >= from)
            {
                TwoWayZ0SCapi.noteFailure(&gp64P->twoWayP->twoWayStatusP);
            }
        }
        
        // If it is KNOW_NO then the output overwrote the input else...
        if(gp64P->twoWayP->twoWayStatusP != KNOW_NO_ARC)
        {
            // ...no showstopper is seen so good (guarantee by setting must work before success).
            TwoWayZ0SCapi.setMustWork(&gp64P->twoWayP->twoWayStatusP);
            TwoWayZ0SCapi.noteSuccess(&gp64P->twoWayP->twoWayStatusP);
        }
    }
    else
    {
        TwoWayZ0SCapi.noteFailure(&gp64P->twoWayP->twoWayStatusP);
    }
}


//Meta
/**
 * Meta data
 * - argument from is the buffer address where the next csv row is to detect overlap
 */
static void
FHO0_meta(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UCI_ROLEINREC) ||
       fieldTrkr & (1 << UCI_RELTOHEAD) ||
       fieldTrkr & (1 << UCI_PVDDID))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;

        strcpy(outP, "=wFHUMeta");
        outP += strlen(outP);

        FHO0_makeOneCol(&outP, UCI_ROLEINREC,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_RELTOHEAD,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_PVDDID,        fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC){
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END Meta
}

/**
 * Birth (from birth record or other source X)
 */
static void
FHO0_birth(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UCI_BDT) || fieldTrkr & (1 << UCI_BPLC))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;

        if(fieldTrkr & (1 << UCI_CDT) || fieldTrkr & (1 << UCI_CPLC) ||
           fieldTrkr & (1 << UCI_MDT) || fieldTrkr & (1 << UCI_MPLC) ||
           fieldTrkr & (1 << UCI_DDT) || fieldTrkr & (1 << UCI_DPLC) ||
           fieldTrkr & (1 << UCI_IDT) || fieldTrkr & (1 << UCI_IPLC))
        {
            strcpy(outP, "=wFHX2B");
        }
        else
        {
            strcpy(outP, "=wFHBir");
        }
        outP += strlen(outP);
        
        FHO0_makeOneCol(&outP, UCI_FULLNM,   fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_GNDR,     fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_BDT,      fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_BPLC,     fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_FFNM,     fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_MFNM,     fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_PVDDID,    fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC){
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END Birthday
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
        FHO0_makeOneCol(&outP, UCI_FULLNM,  fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_CDT,     fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_CPLC,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_FFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_MFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_PVDDID,   fieldTrkr);
        
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
        FHO0_makeOneCol(&outP, UCI_FULLNM,  fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_MDT,     fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_MPLC,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_FFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_MFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_SFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_PVDDID,   fieldTrkr);
        
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
        
        FHO0_makeOneCol(&outP, UCI_FULLNM,  fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_GNDR,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_DDT,     fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_DPLC,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_FFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_MFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_SFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_PVDDID,   fieldTrkr);
        
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
        
        FHO0_makeOneCol(&outP, UCI_FULLNM,  fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_IDT,     fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_IPLC,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_FFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_MFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_SFNM,    fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_PVDDID,   fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END Burial/Internment
}

// Other
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
        FHO0_makeOneCol(&outP, UCI_CFNMS,   fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_OFNMS,   fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_PFNMS,   fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_OEVENTS, fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END Other
}

/**
 * Gather all the batch numbers.
 */
static void
FHO0_batchIx(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UCI_BCHNBR))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(outP, "=wFHBatchId");
        outP += strlen(outP);
        FHO0_makeOneCol(&outP, UCI_BCHNBR,   fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END BatchIx
}

static Ulng
FHO0_bestDate(char* outP, Ullg fieldTrkr, char* from)
{
    char* keepOutP = outP;
    
    if(fieldTrkr & (1 << UCI_BDT)){
        FHO0_makeOneCol(&outP, UCI_BDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_CDT)){
        FHO0_makeOneCol(&outP, UCI_CDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_MDT)){
        FHO0_makeOneCol(&outP, UCI_MDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_DDT)){
        FHO0_makeOneCol(&outP, UCI_DDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_IDT)){
        FHO0_makeOneCol(&outP, UCI_IDT,   fieldTrkr);
    }else if(fieldTrkr & (1 << UCI_RESDT)){
        FHO0_makeOneCol(&outP, UCI_RESDT,   fieldTrkr);
    }else{
        int dnvCtr = uciToDnv[UCI_RESDT]; // redirect columnIdUniversal to dictionaryAndValue index
        FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
        fieldTrkr |= (1<<UCI_RESDT); // local
        dnvP->value = "00 Unk 0000";
        dnvP->length = strlen(dnvP->value);
        FHO0_makeOneCol(&outP, UCI_RESDT, fieldTrkr);
    }
    
    keepOutP += 2;
    Ulng bestDate = strtol(keepOutP, NO_ARG_PTR_ARC, RADIX_10_ARC);
    if(bestDate > 1000){
        bestDate *= 100 * 100; // Only a year is given so finish it
    }else if(bestDate <= 31){
        // this is a day of the month so keep it and move to the alpha month
        bestDate += DictO0SCapi.getOfAlphaMon(keepOutP) * 100;
        bestDate += strtol(outP - 4, NO_ARG_PTR_ARC, RADIX_10_ARC) * 100 * 100;
    }
    
    return bestDate;
}

/**
 * Gather all the fullName, eventDate, BatchIx
 */
static void
FHO0_nmDtBatchId(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UCI_BCHNBR) ||
       fieldTrkr & (1 << UCI_PVDDID)  ||
       fieldTrkr & (1 << UCI_FULLNM) ||
       fieldTrkr & (1 << UCI_BDT) ||
       fieldTrkr & (1 << UCI_CDT) ||
       fieldTrkr & (1 << UCI_MDT) ||
       fieldTrkr & (1 << UCI_DDT) ||
       fieldTrkr & (1 << UCI_IDT))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(outP, "=wFHNmDtBch");
        outP += strlen(outP);
        FHO0_makeOneCol(&outP, UCI_SCORE,   fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_FULLNM,  fieldTrkr);
        FHO0_bestDate(outP, fieldTrkr, from); // also returns CCYYMMDD
//        char wk[100];
//        sprintf(wk, "%08lu", FHO0_bestDate(outP, fieldTrkr, from));
//        printf(">>>%s\n", wk);
        
//        if(fieldTrkr & (1 << UCI_BDT)){
//            FHO0_makeOneCol(&outP, UCI_BDT,   fieldTrkr);
//        }else if(fieldTrkr & (1 << UCI_CDT)){
//            FHO0_makeOneCol(&outP, UCI_CDT,   fieldTrkr);
//        }else if(fieldTrkr & (1 << UCI_MDT)){
//            FHO0_makeOneCol(&outP, UCI_MDT,   fieldTrkr);
//        }else if(fieldTrkr & (1 << UCI_DDT)){
//            FHO0_makeOneCol(&outP, UCI_DDT,   fieldTrkr);
//        }else if(fieldTrkr & (1 << UCI_IDT)){
//            FHO0_makeOneCol(&outP, UCI_IDT,   fieldTrkr);
//        }else if(fieldTrkr & (1 << UCI_RESDT)){
//            FHO0_makeOneCol(&outP, UCI_RESDT,   fieldTrkr);
//        }else{
//            int dnvCtr = uciToDnv[UCI_RESDT]; // redirect columnIdUniversal to dictionaryAndValue index
//            FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
//            fieldTrkr |= (1<<UCI_RESDT); // local
//            dnvP->value = "00 Unk 0000";
//            dnvP->length = strlen(dnvP->value);
//            FHO0_makeOneCol(&outP, UCI_RESDT, fieldTrkr);
//        }
        
        if(fieldTrkr & (1 << UCI_BPLC)){
            FHO0_makeOneCol(&outP, UCI_BPLC,   fieldTrkr);
        }else if(fieldTrkr & (1 << UCI_CPLC)){
            FHO0_makeOneCol(&outP, UCI_CPLC,   fieldTrkr);
        }else if(fieldTrkr & (1 << UCI_MPLC)){
            FHO0_makeOneCol(&outP, UCI_MPLC,   fieldTrkr);
        }else if(fieldTrkr & (1 << UCI_DPLC)){
            FHO0_makeOneCol(&outP, UCI_DPLC,   fieldTrkr);
        }else if(fieldTrkr & (1 << UCI_IPLC)){
            FHO0_makeOneCol(&outP, UCI_IPLC,   fieldTrkr);
        }else if(fieldTrkr & (1 << UCI_RESPLC)){
            FHO0_makeOneCol(&outP, UCI_RESPLC,   fieldTrkr);
        }else{
            int dnvCtr = uciToDnv[UCI_RESPLC]; // redirect columnIdUniversal to dictionaryAndValue index
            FHZ0DictionaryAndValuePT dnvP = &FHZ0DictionaryAndValue[dnvCtr];
            fieldTrkr |= (1<<UCI_RESPLC); // local
            dnvP->value = "noPlace";
            dnvP->length = strlen(dnvP->value);
            FHO0_makeOneCol(&outP, UCI_RESPLC, fieldTrkr);
        }
        
        FHO0_makeOneCol(&outP, UCI_PVDDID,   fieldTrkr);
        FHO0_makeOneCol(&outP, UCI_BCHNBR,  fieldTrkr);
        
        FHO0_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHZ0control.linePresentingError = __LINE__;
        }
    }//END NameDateId
}

/**
 * Contains as rows all the data that is in the read export csv file.
 */
static void
FHO0_newFile(char* path, fileWoTypeT file, gpSllgChar64PT gp64P)
{
    TwoWayZ0SCapi.setMustWork(&gp64P->twoWayP->twoWayStatusP);
    
    FHO0_OpenReadClose(path, file, gp64P);
    
    FHZ0control.currWrite = FHZ0control.currentRead = FHZ0control.buf;
    FHZ0control.linePresentingError = 0;
    FHZ0control.droppedCount = 0;
    
    char *tokenNxtP = strstr(FHZ0control.buf + FHXR_HDSTART, FHXRCH_BEGIN); // TODO: depends on file source
    
    if(gp64P->twoWayP->twoWayStatusP == KNOW_YES_ARC)
    {
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
        int doExtraTimeIn = 1; // to also get the output after the final input
        colHdrHashCtl.tokenEndP = colHdrHashCtl.tokenNxtP; // prevent null address.
        while(*colHdrHashCtl.tokenNxtP || doExtraTimeIn)
        {
            if(strchr(rowSeps, *(colHdrHashCtl.tokenEndP+1)) != 0)
            {
                // We are at a row separator.
                if(FHZ0control.rowNbr > 0){
                    // Programming note: fieldTrkr is 0 on the header,
                    // but coding the ifRowNbr above feels safer and reduces work.
                    
                    FHO0_batchIx(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                    FHO0_nmDtBatchId(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                    
//                    FHO0_meta (fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
//                    FHO0_birth(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
//                    FHO0_chris(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
//                    FHO0_marry(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
//                    FHO0_death(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
//                    FHO0_bury (fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
//                    FHO0_other(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                }
                
                // If got here as final entry due to doExtraTimeIn
                // then quit voluntairly because the input is used up.
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
            else // ... or an input row is being processed.
            {
                if(colHdrHashCtl.tokenBegP != colHdrHashCtl.tokenEndP){
                    fieldTrkr += 1 << FHZ0DictionaryAndValue[colToDnv[FHZ0control.colNbr]].uci;
                }
                
                FHO0_setColVal(FHZ0control.colNbr, colHdrHashCtl.tokenBegP, colHdrHashCtl.tokenEndP);
                // Programming note: To watch a particular row,
                // put the desired row number in place of the zero.
                if(FHZ0control.rowNbr == 0)
                {
                    FHO0_describe(__LINE__, FHZ0control.colNbr, FHZ0control.rowNbr);
                }
            }
            
            FHZ0control.colNbr++;
        }//END while more CSV
    }
    else{
        FHZ0control.linePresentingError = __LINE__;
    }
}


FHO0ACapiT FHO0ACapi =
{
    FHO0_newFile
};

// END FamilyHistoryZ0.c
/**
 *
 */
