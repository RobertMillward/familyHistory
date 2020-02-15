//
//  FH_FSExportReader.c
//
//  Created by Robert R on 2/9/15.
//  Copyright (c) 2020 Robert Russell Millward.  All rights reserved.
//
// os
//#include <sys/types.h>
//#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include "ArchitectureAbugXC.h"
// helper and service api's
#include "HashBasicsO0.h"
#include "RowO0.h"
// data plans
#include "FH_FSExportReader.h"

//#include "SignatureFactoryO0.h"
//static nameSigPT mySigInfoP = 0; // {name, newCount, otherCount, signature, {versionLevel}}
//static void
//selfInit()
//{
//    if(mySigInfoP == 0)
//    {
//        mySigInfoP = SignatureFactoryC.getInfoByName(__FILE__);
//    }
//}

/**
 * An internal class to support putting this non-const data
 * into the stack.
 */


/**
 * The column number in the input.
 * Destination row is in the comment.
 * Current headers in order as of 02/15/2019:
 */






typedef struct FamilySearchExportColHdrsS
{
    struct fieldCtlAryS
    {
        char  colName[24];
        columnNbrInET colE;
        char *colP;
        int   colSz;
    }fieldCtlAry[CNI_MAX_FLD];
    
    void (*setColVal)(int col,
                      char *beg,
                      char *end,
                      struct FamilySearchExportColHdrsS *this);
    void (*makeOneCol)(char **outP,
                       int forCol,
                       int fieldTrkr,
                       struct FamilySearchExportColHdrsS *this);
    void (*describe)(int line,
                     int fieldCtr,
                     int rowCtr,
                     struct FamilySearchExportColHdrsS *this);
    int (*check)(int fieldCtr,
                 char *begP,
                 struct FamilySearchExportColHdrsS *this);
    void (*getIndexReason)(char *here,
                  struct FamilySearchExportColHdrsS *this);
    
}FamilySearchExportColHdrs, *FamilySearchExportColHdrsP;


static void
setColVal(int col, char *beg, char *end, FamilySearchExportColHdrsP this)
{
    //selfInit(); // eliminate compiler warning
    if(*beg == '"')
    {
        beg++;
        end--;
    }
    this->fieldCtlAry[col].colP = beg;
    this->fieldCtlAry[col].colSz = (int)(end - beg);
}

static void
makeOneCol(char **outP, int forCol, int fieldTrkr, FamilySearchExportColHdrsP this)
{
    if(fieldTrkr & (1 << forCol))
    {
        sprintf(*outP,
                "%.*s=%c",
                this->fieldCtlAry[forCol].colSz,
                this->fieldCtlAry[forCol].colP,
                this->fieldCtlAry[forCol].colName[FHA_COL_IN_ROW]);
        *outP += strlen(*outP);
    }
}

static void
describe(int line, int fieldCtr, int rowCtr, FamilySearchExportColHdrsP this)
{
    printf("%4i [%4d] %-18s '%.*s'\n",
       line,
       rowCtr,
       this->fieldCtlAry[fieldCtr].colName,
       this->fieldCtlAry[fieldCtr].colSz,
       this->fieldCtlAry[fieldCtr].colP);
}


static int
check(int fieldCtr, char *begP, FamilySearchExportColHdrsP this)
{
    return strncmp(this->fieldCtlAry[fieldCtr].colName + FHA_COL_ID_LEN,
                   begP,
                   strlen(this->fieldCtlAry[fieldCtr].colName) - FHA_COL_ID_LEN);
}

static void
getIndexAndWhy(char *here, FamilySearchExportColHdrsP this)
{
    sprintf(here,
            "%.*s=x%c=y",
            this->fieldCtlAry[CNI_KEY].colSz,
            this->fieldCtlAry[CNI_KEY].colP,
            ACTIVE_ROWST);
}



FamilySearchExportColHdrs newFamilySearchExportColHdrs()
{
    FamilySearchExportColHdrs newItem =
    {
        {
            {ITEM_NAME_STR_ROW "xx" FHXRCH_BEGIN,   CNI_KEY}, // the "w" part, score
            {FHA_COL_PRINM "full_name",             CNI_FNAME},
            {FHA_COL_PRIGN "gender",                CNI_GNDR},
            // Note that only one date and location will be active per output
            // These translations become event date and event location.
            {FHA_COL_PRIDT "birth_date",            CNI_BDT},
            {FHA_COL_PRIPL "birth_place_text",      CNI_BPLC},
            {FHA_COL_PRIDT "chr_date",              CNI_CDT},
            {FHA_COL_PRIPL "chr_place_text",        CNI_CPLC},
            {FHA_COL_PRIDT "residence_date",        CNI_RESDT},
            {FHA_COL_PRIPL "residence_place_text",  CNI_RESPLC},
            {FHA_COL_PRIDT "marriage_date",         CNI_MDT},
            {FHA_COL_PRIPL "marriage_place_text",   CNI_MPLC},
            {FHA_COL_PRIDT "death_date",            CNI_DDT},
            {FHA_COL_PRIPL "death_place_text",      CNI_DPLC},
            {FHA_COL_PRIDT "burial_date",           CNI_IDT},
            {FHA_COL_PRIPL "burial_place_text",     CNI_IPLC},
            //
            {FHA_COL_FTHNM "father_full_name",      CNI_FFNM},
            {FHA_COL_MTHNM "mother_full_name",      CNI_MFNM},
            {FHA_COL_SPONM "spouse_full_name",      CNI_SFNM},
            //
            {FHA_COL_OTHCFNM "children_full_names", CNI_CFNMS},
            {FHA_COL_OTHOFNM "other_full_names",    CNI_OFNMS},
            {FHA_COL_OTHPFNM "parent_full_names",   CNI_PFNMS},
            {FHA_COL_OTHEVNT "other_events",        CNI_OEVENTS},
            //
            {FHA_COL_MTABTCH "batch_number",        CNI_BATCHNBR},
            {FHA_COL_MTAROLE "role_in_record",      CNI_ROLEINREC},
            {FHA_COL_MTARELT "relationship_to_head",CNI_RELTOHEAD},
            {FHA_COL_MTAPURL "person_url",          CNI_IURL},
            {FHA_COL_MTSRCTP "source_media_type",   CNI_SMT},
        },
        
        setColVal,
        makeOneCol,
        describe,
        check,
        getIndexAndWhy // inde[x] and reason-wh[y]
    };
    
    return newItem;
}



/**
 * Back to the target class.
 */
static char*
fxhrOpenReadClose(char *fullPath, FHFSExportReaderP this)
{
    char *retStr = KNOW_MAYBE_ARC;
    fdT myfd = open(fullPath, O_RDONLY);
    if(myfd < 0)
    {
        retStr = KNOW_NO_ARC;
    }
    else
    {
        int maxReadLen = FHXR_BUF_SZ - FHXR_HDSTART;
        ssize_t rdlen = read(myfd, this->buf + FHXR_HDSTART, maxReadLen);
        if(rdlen == maxReadLen)
        {
            retStr = KNOW_NO_ARC;
        }
        else
        {
            this->buf[rdlen + FHXR_HDSTART] = '\0';
        }
        
        close(myfd);
    }
    
    if(retStr != KNOW_NO_ARC)
    {
        retStr = KNOW_YES_ARC;
    }
    
    return retStr;
}


static char*
checkThenPutInfo(int line,
                 char *record,
                 char *from,
                 FamilySearchExportColHdrsP hdrsP,
                 FHFSExportReaderP this)
{
    char *retStr = KNOW_MAYBE_ARC;
    
    if(strlen(record) < FHXR_OUTSZ)
    {
        //printf("%4i '%s'\n", line, record);
        
        char *dupChkP = this->buf;
        while(dupChkP < this->currWrite)
        {
            if(strncmp(dupChkP, record, strlen(record)) == 0)
            {
                // Don't store duplicates
                this->droppedCount++;
                retStr = KNOW_YES_ARC;
                break;
            }
            
            dupChkP += strlen(dupChkP) + 1;
        }
        
        // MAYBE is non-duplicate for ease of use.
        if(retStr == KNOW_MAYBE_ARC)
        {
            strcpy(this->currWrite, record);
            this->currWrite += strlen(this->currWrite);
            
            hdrsP->getIndexReason(this->currWrite, hdrsP);
            
            // Advance pointer and check for overlap.
            this->currWrite += strlen(this->currWrite) + 1;
            if(this->currWrite >= from)
            {
                retStr = KNOW_NO_ARC;
            }
        }
        
        // If it is KNOW_NO then the output overwrote the input else...
        if(retStr != KNOW_NO_ARC)
        {
            // ...no showstopper is seen so good.
            retStr = KNOW_YES_ARC;
        }
    }
    else
    {
        retStr = DONT_CARE_ARC;
    }
    
    return retStr;
}


/**
 * Contains all the data that is in the export file.
 */
FHFSExportReader newFHFSExportReader(char *fullPath)
{
    FHFSExportReader newItem;
    
    char *retStr = fxhrOpenReadClose(fullPath, &newItem);
    newItem.currWrite = newItem.currentRead = newItem.buf;
    newItem.linePresentingError = 0;
    newItem.droppedCount = 0;
    
    char *tokenNxtP = strstr(newItem.buf + FHXR_HDSTART, FHXRCH_BEGIN);
    FamilySearchExportColHdrs fsColCtl = newFamilySearchExportColHdrs();
    
    if(retStr == KNOW_YES_ARC)
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
        
        
        int rowCtr = 0;
        int fieldCtr = 0;
        int fieldTrkr = 0;
        int doExtraTimeIn = 1; // to also get the output after the final input
        while(*colHdrHashCtl.tokenNxtP || doExtraTimeIn)
        {
            if(fieldCtr == CNI_MAX_FLD &&
               strchr(rowSeps, *(colHdrHashCtl.tokenEndP)) != 0)
            {
                // Make a record
                char record[FHXR_OUTSZ] = "";
                char *outP = record;
                
                //Meta
                if(fieldTrkr & (1 << CNI_BATCHNBR) || fieldTrkr & (1 << CNI_ROLEINREC) ||
                   fieldTrkr & (1 << CNI_RELTOHEAD) || fieldTrkr & (1 << CNI_IURL))
                {
                    strcpy(record, "FHMta=w");
                    outP = record;
                    outP += strlen(outP);
                    
                    fsColCtl.makeOneCol(&outP, CNI_BATCHNBR,     fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_ROLEINREC,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_RELTOHEAD,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_IURL,         fieldTrkr, &fsColCtl);
                    
                    retStr = checkThenPutInfo(__LINE__,
                                              record,
                                              colHdrHashCtl.tokenBegP,
                                              &fsColCtl,
                                              &newItem);
                    if(retStr == KNOW_NO_ARC)
                    {
                        newItem.linePresentingError = __LINE__;
                        break;
                    }
                    strcpy(record, "");
                }//END Meta
                
                // Birth (from birth record or other source X)
                if(fieldTrkr & (1 << CNI_BDT) || fieldTrkr & (1 << CNI_BPLC))
                {
                    if(fieldTrkr & (1 << CNI_CDT) || fieldTrkr & (1 << CNI_CPLC) ||
                       fieldTrkr & (1 << CNI_MDT) || fieldTrkr & (1 << CNI_MPLC) ||
                       fieldTrkr & (1 << CNI_DDT) || fieldTrkr & (1 << CNI_DPLC) ||
                       fieldTrkr & (1 << CNI_IDT) || fieldTrkr & (1 << CNI_IPLC))
                    {
                        strcpy(record, "FHX2B=w");
                    }
                    else
                    {
                        strcpy(record, "FHBir=w");
                    }
                    outP = record;
                    outP += strlen(outP);
                    
                    fsColCtl.makeOneCol(&outP, CNI_FNAME,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_GNDR,     fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_BDT,      fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_BPLC,     fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_FFNM,     fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_MFNM,     fieldTrkr, &fsColCtl);
                    
                    retStr = checkThenPutInfo(__LINE__,
                                              record,
                                              colHdrHashCtl.tokenBegP,
                                              &fsColCtl,
                                              &newItem);
                    if(retStr == KNOW_NO_ARC)
                    {
                        newItem.linePresentingError = __LINE__;
                        break;
                    }
                    strcpy(record, "");
                }//END Birthday
                
                // Christening
                if(fieldTrkr & (1 << CNI_CDT) || fieldTrkr & (1 << CNI_CPLC))
                {
                    strcpy(record, "FHChr=w");
                    outP = record;
                    outP += strlen(outP);
                    fsColCtl.makeOneCol(&outP, CNI_FNAME,   fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_CDT,     fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_CPLC,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_FFNM,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_MFNM,    fieldTrkr, &fsColCtl);

                    retStr = checkThenPutInfo(__LINE__,
                                              record,
                                              colHdrHashCtl.tokenBegP,
                                              &fsColCtl,
                                              &newItem);
                    if(retStr == KNOW_NO_ARC)
                    {
                        newItem.linePresentingError = __LINE__;
                        break;
                    }
                    strcpy(record, "");
                }//END Christening
                
                // Marriage
                if(fieldTrkr & (1 << CNI_MDT) || fieldTrkr & (1 << CNI_MPLC) ||
                   fieldTrkr & (1 << CNI_SFNM))
                {
                    strcpy(record, "FHMar=w");
                    outP = record;
                    outP += strlen(outP);
                    fsColCtl.makeOneCol(&outP, CNI_FNAME,   fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_MDT,     fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_MPLC,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_FFNM,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_MFNM,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_SFNM,    fieldTrkr, &fsColCtl);
                    
                    retStr = checkThenPutInfo(__LINE__,
                                              record,
                                              colHdrHashCtl.tokenBegP,
                                              &fsColCtl,
                                              &newItem);
                    if(retStr == KNOW_NO_ARC)
                    {
                        newItem.linePresentingError = __LINE__;
                        break;
                    }
                    strcpy(record, "");
                }//END Marriage
                
                // Death (from birth record or other source X)
                if(fieldTrkr & (1 << CNI_DDT) || fieldTrkr & (1 << CNI_DPLC))
                {
                    if(
                       fieldTrkr & (1 << CNI_BDT) || fieldTrkr & (1 << CNI_BPLC) ||
                       fieldTrkr & (1 << CNI_CDT) || fieldTrkr & (1 << CNI_CPLC) ||
                       fieldTrkr & (1 << CNI_MDT) || fieldTrkr & (1 << CNI_MPLC) ||
                       fieldTrkr & (1 << CNI_IDT) || fieldTrkr & (1 << CNI_IPLC))
                    {
                        strcpy(record, "FHX2D=w"); // some other record also has death date
                    }
                    else
                    {
                        strcpy(record, "FHDea=w");
                    }
                    outP = record;
                    outP += strlen(outP);
                    
                    fsColCtl.makeOneCol(&outP, CNI_FNAME,   fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_GNDR,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_DDT,     fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_DPLC,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_FFNM,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_MFNM,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_SFNM,    fieldTrkr, &fsColCtl);
                    
                    retStr = checkThenPutInfo(__LINE__,
                                              record,
                                              colHdrHashCtl.tokenBegP,
                                              &fsColCtl,
                                              &newItem);
                    if(retStr == KNOW_NO_ARC)
                    {
                        newItem.linePresentingError = __LINE__;
                        break;
                    }
                    strcpy(record, "");
                }//END Death
                
                // Burial/Internment
                if(fieldTrkr & (1 << CNI_IDT) || fieldTrkr & (1 << CNI_IPLC))
                {
                    strcpy(record, "FHInt=w");
                    outP = record;
                    outP += strlen(outP);
                    
                    fsColCtl.makeOneCol(&outP, CNI_FNAME,   fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_IDT,     fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_IPLC,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_FFNM,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_MFNM,    fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_SFNM,    fieldTrkr, &fsColCtl);
                    
                    retStr = checkThenPutInfo(__LINE__,
                                              record,
                                              colHdrHashCtl.tokenBegP,
                                              &fsColCtl,
                                              &newItem);
                    if(retStr == KNOW_NO_ARC)
                    {
                        newItem.linePresentingError = __LINE__;
                        break;
                    }
                    strcpy(record, "");
                }//END Burial/Internment

            
                // Other
                if(fieldTrkr & (1 << CNI_CFNMS) ||
                   fieldTrkr & (1 << CNI_OFNMS) ||
                   fieldTrkr & (1 << CNI_PFNMS) ||
                   fieldTrkr & (1 << CNI_OEVENTS))
                {
                    strcpy(record, "FHOth=w");
                    outP = record;
                    outP += strlen(outP);
                    fsColCtl.makeOneCol(&outP, CNI_CFNMS,   fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_OFNMS,   fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_PFNMS,   fieldTrkr, &fsColCtl);
                    fsColCtl.makeOneCol(&outP, CNI_OEVENTS, fieldTrkr, &fsColCtl);
                    
                    retStr = checkThenPutInfo(__LINE__,
                                              record,
                                              colHdrHashCtl.tokenBegP,
                                              &fsColCtl,
                                              &newItem);
                    if(retStr == KNOW_NO_ARC)
                    {
                        newItem.linePresentingError = __LINE__;
                        break;
                    }
                    strcpy(record, "");
                }//END Other
                
                // If got here as final entry in then quit voluntairly.
                if(! *colHdrHashCtl.tokenNxtP)
                    break;
                
                
                // Manage controls and continue
                fieldCtr = 0;
                fieldTrkr = 0;
                rowCtr++;
                continue;
            }
            
            // Back to processing a column...
            /*eightHashT hsh =*/ HashBasicsHCapi.getBasicHash(&colHdrHashCtl);
            
            // Either a column header (from row 0) is being processed...
            if(rowCtr == 0)
            {
                if(fsColCtl.check(fieldCtr, colHdrHashCtl.tokenBegP, &fsColCtl) != 0)
                {
                    newItem.linePresentingError = __LINE__;
                    break;
                }
            }
            else // ... or a data row is being processed.
            {
                if(colHdrHashCtl.tokenBegP != colHdrHashCtl.tokenEndP)
                {
                    //int rowColLtr = fsxColNames[fieldCtr][0];
                    fieldTrkr += 1 << fieldCtr;
                    fsColCtl.setColVal(fieldCtr,
                                       colHdrHashCtl.tokenBegP,
                                       colHdrHashCtl.tokenEndP,
                                       &fsColCtl);
                    // To watch a particular line,
                    // Put the desired row number in place of the zero.
                    if(rowCtr == 0)
                    {
                        fsColCtl.describe(__LINE__, fieldCtr, rowCtr, &fsColCtl);
                    }
                }
            }
            
            fieldCtr++;
        }//END while more CSV
    }
    else
    {
        newItem.linePresentingError = __LINE__;
        
    }
    
    return newItem;
}
// END FH_FSExportReader.c
/**
 *
 */

