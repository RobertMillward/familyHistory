//
//  FamilyHistoryPlan.c
//
//  Created by Robert Russell Millward on 2/15/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
// os
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include "ArchitectureAbugXC.h"
// helper and service api's
#include "FamilyHistoryApi.h"
#include "HashBasicsO0.h"
// data plans
#include "ArchitectureZ0Plan.h"
#include "FamilyHistoryPlan.h"
#include "HashBasicsZ0Plan.h"
#include "RowZ0Plan.h"
//
#define VALHERE 0
#define LENHERE 0

static FHU_DictionaryAndValueT FHU_DictionaryAndValue[] =
{
    {UNI_FNAME,     FHA_COL_PRINM, VALHERE, LENHERE, {"full_name",       0}},
    {UNI_GNDR,      FHA_COL_PRIGN, VALHERE, LENHERE, {"gender",          0}},
    {UNI_BDT,       FHA_COL_PRIDT, VALHERE, LENHERE, {"birth_date",      0}},
    {UNI_BPLC,      FHA_COL_PRIPL, VALHERE, LENHERE, {"birth_place_text",0}},
    {UNI_CDT,       FHA_COL_PRIDT, VALHERE, LENHERE, {"chr_date",        0}},
    {UNI_CPLC,      FHA_COL_PRIPL, VALHERE, LENHERE, {"chr_place_text",  0}},
    
    {UNI_RESDT,     FHA_COL_PRIDT, VALHERE, LENHERE, {"residence_date",      0}},
    {UNI_RESPLC,    FHA_COL_PRIPL, VALHERE, LENHERE, {"residence_place_text",0}},
    {UNI_MDT,       FHA_COL_PRIDT, VALHERE, LENHERE, {"marriage_date",        0}},
    {UNI_MPLC,      FHA_COL_PRIPL, VALHERE, LENHERE, {"marriage_place_text",  0}},
    
    {UNI_DDT,       FHA_COL_PRIDT, VALHERE, LENHERE, {"death_date",      0}},
    {UNI_DPLC,      FHA_COL_PRIPL, VALHERE, LENHERE, {"death_place_text",0}},
    {UNI_IDT,       FHA_COL_PRIDT, VALHERE, LENHERE, {"burial_date",        0}},
    {UNI_IPLC,      FHA_COL_PRIPL, VALHERE, LENHERE, {"burial_place_text",  0}},
    
    {UNI_FFNM,      FHA_COL_FTHNM, VALHERE, LENHERE, {"father_full_name",   0}},
    {UNI_MFNM,      FHA_COL_MTHNM, VALHERE, LENHERE, {"mother_full_name",0}},
    {UNI_SFNM,      FHA_COL_SPONM, VALHERE, LENHERE, {"spouse_full_name",   0}},
    {UNI_PFNMS,     FHA_COL_OTHPFNM, VALHERE, LENHERE, {"parent_full_names",  0}},
    {UNI_CFNMS,     FHA_COL_OTHCFNM, VALHERE, LENHERE, {"children_full_names",0}},
    {UNI_OFNMS,     "Xlv",          VALHERE, LENHERE, {"other_full_names",0}},
    {UNI_OEVENTS,   FHA_COL_OTHEVNT, VALHERE, LENHERE, {"other_events",        0}},
    
    {UNI_SCORE,     "Xlv",          VALHERE, LENHERE, {"score",           0}},
    {UNI_IURL,      "Xlv",          VALHERE, LENHERE, {"person_url",      0}},
    {UNI_SMT,       FHA_COL_MTSRCTP, VALHERE, LENHERE, {"source_media_type", 0}},
    {UNI_ROLEINREC, FHA_COL_MTRECRL, VALHERE, LENHERE, {"role_in_record",  0}},
    {UNI_RELTOHEAD, FHA_COL_MTRLTHD, VALHERE, LENHERE, {"relationship_to_head", 0}},
    {UNI_SUBCLLID,  "Xlv",          VALHERE, LENHERE, {"subcollection_id", 0}},
    {UNI_EUID,      "Xlv",          VALHERE, LENHERE, {"easy_unique_id",  0}},
    {UNI_RURL,      "Xlv",          VALHERE, LENHERE, {"record_url",      0}},
    {UNI_BCHNBR,    FHA_COL_MTABTCH, VALHERE, LENHERE, {"batch_number",    0}},
    {0}
};

FHU_controlT FHU_control;

static int colMap[100];

//static void
//fillAltNames()
//{
//    
//    for(int altNmIx = 0; FamHistDictionaryAndValue[altNmIx].id != 0; altNmIx++)
//    {
//        switch(FamHistDictionaryAndValue[altNmIx].id)
//        {
//            case UNI_SCORE: FamHistDictionaryAndValue[altNmIx].list = &uniScore[0]; break;
//        }
//    }
//}



static void
FHU_setColVal(int fieldCtr, char *beg, char *end)
{
    FHU_DictionaryAndValuePT dataP = &FHU_DictionaryAndValue[colMap[fieldCtr]];
    //FHU_controlPT ctrlP = &FHU_control;
    
    if(*beg == '"')
    {
        beg++;
        end--;
    }
    
    dataP->value = beg;
    dataP->length = (int)(end - beg);
}

static void
FHU_makeOneCol(char **outP, int fieldCtr, Ullg fieldTrkr)
{
    FHU_DictionaryAndValuePT dataP = &FHU_DictionaryAndValue[colMap[fieldCtr]];
    //FHU_controlPT ctrlP = &FHU_control;
    
    if(fieldTrkr & (1 << fieldCtr))
    {
        sprintf(*outP,
                "=%c%.*s",
                dataP->control[FHA_COL_IN_ROW], // field letter
                (int)dataP->length,
                dataP->value);
        *outP += strlen(*outP);
    }
}

static void
FHU_describe(int line, int fieldCtr, int rowCtr)
{
    FHU_DictionaryAndValuePT dataP = &FHU_DictionaryAndValue[colMap[fieldCtr]];
    //FHU_controlPT ctrlP = &FHU_control;
    
    printf("%4i [%4d] %-18s '%.*s'\n",
       line,
       rowCtr,
       dataP->list[0],
       (int)dataP->length,
       dataP->value);
}


static int
FHU_checkColName(int fieldCtr, char *begP)
{
    FHU_DictionaryAndValuePT dataP = &FHU_DictionaryAndValue[colMap[fieldCtr]];
    //FHU_controlPT ctrlP = &FHU_control;
    
    return strncmp(dataP->list[0], // 0-n TODO
                   begP,
                   strlen(dataP->list[0]) - FHA_COL_ID_LEN);
}

/**
 * Provides for the row the source, the index, and a why of ACTIVE_ROWST.
 * The what is provided by the caller.
 */
static void
FHU_getIndexAndWhy(char *here) // what?, indeX, whY, whoZ TODO
{
    //FHU_DictionaryAndValuePT dataP = &FHU_DictionaryAndValue[0];
    FHU_controlPT ctrlP = &FHU_control;
    
    sprintf(here,
            "=x%d=z%s=y%c",
            ctrlP->rowNbr,
            ctrlP->fileName,
            ACTIVE_ROWST);
}

static void
FHU_checkThenPutInfo(int line, char *record, char *from, gpSllgChar64PT gp64P)
{
    //FHU_DictionaryAndValuePT dataP = &FHU_DictionaryAndValue[0];
    FHU_controlPT ctrlP = &FHU_control;
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
                ctrlP->droppedCount++;
                TwoWayZ0SCapi.noteSuccess(&gp64P->twoWayP->twoWayStatusP);
                break;
            }
            
            dupChkP += strlen(dupChkP) + 1;
        }
        
        // MAYBE is non-duplicate for ease of use.
        if(gp64P->twoWayP->twoWayStatusP == KNOW_MAYBE_ARC)
        {
            strcpy(ctrlP->currWrite, record);
            ctrlP->currWrite += strlen(ctrlP->currWrite);
            
            FHU_getIndexAndWhy(ctrlP->currWrite);
            
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
            // ...no showstopper is seen so good.
            TwoWayZ0SCapi.noteSuccess(&gp64P->twoWayP->twoWayStatusP);
        }
    }
    else
    {
        TwoWayZ0SCapi.noteFailure(&gp64P->twoWayP->twoWayStatusP);
    }
}


//Meta
static void
FHU_meta(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UNI_BCHNBR) || // these shifts might be incorrect
       fieldTrkr & (1 << UNI_ROLEINREC) ||
       fieldTrkr & (1 << UNI_RELTOHEAD) ||
       fieldTrkr & (1 << UNI_IURL))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;

        strcpy(record, "=wFHUMeta");
        outP = record;
        outP += strlen(outP);
        
        FHU_makeOneCol(&outP, UNI_BCHNBR,       fieldTrkr);
        FHU_makeOneCol(&outP, UNI_ROLEINREC,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_RELTOHEAD,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_IURL,         fieldTrkr);
        // int line, char *record, char *from
        FHU_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC){
            FHU_control.linePresentingError = __LINE__;
        }
        strcpy(record, "");
    }//END Meta
}

// Birth (from birth record or other source X)
static void
FHU_birth(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UNI_BDT) || fieldTrkr & (1 << UNI_BPLC))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;

        if(fieldTrkr & (1 << UNI_CDT) || fieldTrkr & (1 << UNI_CPLC) ||
           fieldTrkr & (1 << UNI_MDT) || fieldTrkr & (1 << UNI_MPLC) ||
           fieldTrkr & (1 << UNI_DDT) || fieldTrkr & (1 << UNI_DPLC) ||
           fieldTrkr & (1 << UNI_IDT) || fieldTrkr & (1 << UNI_IPLC))
        {
            strcpy(record, "=wFHX2B");
        }
        else
        {
            strcpy(record, "=wFHBir");
        }
        outP = record;
        outP += strlen(outP);
        
        FHU_makeOneCol(&outP, UNI_FNAME,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_GNDR,     fieldTrkr);
        FHU_makeOneCol(&outP, UNI_BDT,      fieldTrkr);
        FHU_makeOneCol(&outP, UNI_BPLC,     fieldTrkr);
        FHU_makeOneCol(&outP, UNI_FFNM,     fieldTrkr);
        FHU_makeOneCol(&outP, UNI_MFNM,     fieldTrkr);
        
        FHU_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC){
            FHU_control.linePresentingError = __LINE__;
        }
        strcpy(record, "");
    }//END Birthday
}

// Christening
static void
FHU_chris(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UNI_CDT) || fieldTrkr & (1 << UNI_CPLC))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(record, "FHChr=w");
        outP = record;
        outP += strlen(outP);
        FHU_makeOneCol(&outP, UNI_FNAME,   fieldTrkr);
        FHU_makeOneCol(&outP, UNI_CDT,     fieldTrkr);
        FHU_makeOneCol(&outP, UNI_CPLC,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_FFNM,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_MFNM,    fieldTrkr);
        
        FHU_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHU_control.linePresentingError = __LINE__;
        }
        strcpy(record, "");
    }//END Christening
}



// Marriage
static void
FHU_marry(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UNI_MDT) || fieldTrkr & (1 << UNI_MPLC) ||
       fieldTrkr & (1 << UNI_SFNM))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(record, "=wFHMar");
        outP = record;
        outP += strlen(outP);
        FHU_makeOneCol(&outP, UNI_FNAME,   fieldTrkr);
        FHU_makeOneCol(&outP, UNI_MDT,     fieldTrkr);
        FHU_makeOneCol(&outP, UNI_MPLC,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_FFNM,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_MFNM,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_SFNM,    fieldTrkr);
        
        FHU_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHU_control.linePresentingError = __LINE__;
        }
        strcpy(record, "");
    }//END Marriage
}

// Death (from birth record or other source X)
static void
FHU_death(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UNI_DDT) || fieldTrkr & (1 << UNI_DPLC))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        if(fieldTrkr & (1 << UNI_BDT) || fieldTrkr & (1 << UNI_BPLC) ||
           fieldTrkr & (1 << UNI_CDT) || fieldTrkr & (1 << UNI_CPLC) ||
           fieldTrkr & (1 << UNI_MDT) || fieldTrkr & (1 << UNI_MPLC) ||
           fieldTrkr & (1 << UNI_IDT) || fieldTrkr & (1 << UNI_IPLC))
        {
            strcpy(record, "=wFHX2D"); // some other record also has death date
        }
        else
        {
            strcpy(record, "=wFHDea");
        }
        outP = record;
        outP += strlen(outP);
        
        FHU_makeOneCol(&outP, UNI_FNAME,   fieldTrkr);
        FHU_makeOneCol(&outP, UNI_GNDR,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_DDT,     fieldTrkr);
        FHU_makeOneCol(&outP, UNI_DPLC,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_FFNM,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_MFNM,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_SFNM,    fieldTrkr);
        
        FHU_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHU_control.linePresentingError = __LINE__;
        }
        strcpy(record, "");
    }//END Death
}

// Burial/Internment
static void
FHU_bury(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UNI_IDT) || fieldTrkr & (1 << UNI_IPLC))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(record, "=wFHInt");
        outP = record;
        outP += strlen(outP);
        
        FHU_makeOneCol(&outP, UNI_FNAME,   fieldTrkr);
        FHU_makeOneCol(&outP, UNI_IDT,     fieldTrkr);
        FHU_makeOneCol(&outP, UNI_IPLC,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_FFNM,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_MFNM,    fieldTrkr);
        FHU_makeOneCol(&outP, UNI_SFNM,    fieldTrkr);
        
        FHU_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHU_control.linePresentingError = __LINE__;
        }
        strcpy(record, "");
    }//END Burial/Internment
}

// Other
static void
FHU_other(Ullg fieldTrkr, char* from, gpSllgChar64PT gp64P)
{
    if(fieldTrkr & (1 << UNI_CFNMS) ||
       fieldTrkr & (1 << UNI_OFNMS) ||
       fieldTrkr & (1 << UNI_PFNMS) ||
       fieldTrkr & (1 << UNI_OEVENTS))
    {
        char record[FHXR_OUTSZ] = "";
        char *outP = record;
        
        strcpy(record, "FHOth=w");
        outP = record;
        outP += strlen(outP);
        FHU_makeOneCol(&outP, UNI_CFNMS,   fieldTrkr);
        FHU_makeOneCol(&outP, UNI_OFNMS,   fieldTrkr);
        FHU_makeOneCol(&outP, UNI_PFNMS,   fieldTrkr);
        FHU_makeOneCol(&outP, UNI_OEVENTS, fieldTrkr);
        
        FHU_checkThenPutInfo(__LINE__, record, from, gp64P);
        if(gp64P->twoWayP->twoWayStatusP == KNOW_NO_ARC)
        {
            FHU_control.linePresentingError = __LINE__;
        }
        strcpy(record, "");
    }//END Other
}


static void
FHU_OpenReadClose(char *path, fileWoTypeT fileWoType, gpSllgChar64PT gp64P)
{
    //FHU_DictionaryAndValuePT dataP = &FHU_DictionaryAndValue;
    FHU_controlPT ctrlP = &FHU_control;

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
 * Contains as rows all the data that is in the read export csv file.
 */
static void
FHU_newFile(char* path, fileWoTypeT file, gpSllgChar64PT gp64P)
{
    TwoWayZ0SCapi.setMustWork(&gp64P->twoWayP->twoWayStatusP);
    
    FHU_OpenReadClose(path, file, gp64P);
    
    FHU_control.currWrite = FHU_control.currentRead = FHU_control.buf;
    FHU_control.linePresentingError = 0;
    FHU_control.droppedCount = 0;
    
    char *tokenNxtP = strstr(FHU_control.buf + FHXR_HDSTART, FHXRCH_BEGIN); // depends on file source TODO
    //FamilySearchExportColHdrs fsColCtl = newFamilySearchExportColHdrs();
    
    if(gp64P->twoWayP->twoWayStatusP == KNOW_YES_ARC)
    {
        int   doAtLeast = 6;
        int   hashMod = HB_BIG_HASH_RSVD;
        int   maxShift = 6;
        int   condense = HB_CONDENSE;
        int   byColumn = HB_COL_NBR_BASE;
        char *fieldSeps = ",";
        char *rowSeps = "\n\r";
        
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
        
        
        FHU_control.rowNbr = 0;
        FHU_control.colNbr = 0;
        unsigned long long fieldTrkr = 0;
        int doExtraTimeIn = 1; // to also get the output after the final input
        while(*colHdrHashCtl.tokenNxtP || doExtraTimeIn)
        {
            if(FHU_control.colNbr == UNI_MAX_FLD &&
               strchr(rowSeps, *(colHdrHashCtl.tokenEndP)) != 0)
            {
                FHU_meta (fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                FHU_birth(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                FHU_chris(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                FHU_marry(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                FHU_death(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                FHU_bury (fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                FHU_other(fieldTrkr, colHdrHashCtl.tokenBegP, gp64P);
                
                // If got here as final entry in then quit voluntairly.
                if(! *colHdrHashCtl.tokenNxtP){
                    break;
                }
                
                // Manage controls and continue
                FHU_control.colNbr = 0;
                fieldTrkr = 0;
                FHU_control.rowNbr++;
                continue;
            }
            
            // Back to processing a column...
            /*eightHashT hsh =*/ HashBasicsHCapi.getBasicHash(&colHdrHashCtl);
            
            // Either a column header (from row 0) is being processed...
            if(FHU_control.rowNbr == 0)
            {
                if(FHU_checkColName(FHU_control.colNbr, colHdrHashCtl.tokenBegP) != 0)
                {
                    FHU_control.linePresentingError = __LINE__;
                    break;
                }
            }
            else // ... or a data row is being processed.
            {
                if(colHdrHashCtl.tokenBegP != colHdrHashCtl.tokenEndP)
                {
                    //int rowColLtr = fsxColNames[fieldCtr][0];
                    fieldTrkr += 1 << FHU_control.colNbr;
                    FHU_setColVal(FHU_control.colNbr, colHdrHashCtl.tokenBegP, colHdrHashCtl.tokenEndP);
                    // Programming note: To watch a particular line,
                    // put the desired row number in place of the zero.
                    if(FHU_control.rowNbr == 0)
                    {
                        FHU_describe(__LINE__, FHU_control.colNbr, FHU_control.rowNbr);
                    }
                }
            }
            
            FHU_control.colNbr++;
        }//END while more CSV
    }
    else{
        FHU_control.linePresentingError = __LINE__;
    }
}


FHUHCapiT FHUHCapi =
{
    FHU_newFile
};

// END FamilyHistoryPlan.c
/**
 *
 */
