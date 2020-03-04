//
//  FamilyHistoryZ3TestAid.c
//  2020Mar10Dev
//
//  Created by Robert Russell Millward on 02/25/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
// os
#include <string.h>
// helper and service api's
#include "CompareO0.h"
#include "CursorO0.h"
//#import "RowO0.h"
#include "FamilyHistoryO0.h"
// data plans
#include "ArchitectureZ0Plan.h"
#include "ArchFamilyHistory.h"
#include "FamilyHistoryZ0Plan.h"
#include "FamilyHistoryZ3Plan.h"
// quality
#include "TestAidZ0.h"
#include "FamilyHistoryZ3TestAid.h"

static void
FHZX_setupSuite0_xf99(lineNbrT lineNbr)
{
    //ErrorHelperO0SCapi.set();
}

static void
FHZX_teardownSuite0_xf99(lineNbrT lineNbr)
{
    
}

static void
FHZX_setupAny_xf99(lineNbrT lineNbr)
{
    TestAidQCapi.basicInit();
}

static bool
FHZX_isScheduled_xf99(cfuncNameT cfuncName, lineNbrT lineNbr)
{
    return TestAidC.isScheduled(cfuncName, TestAidZ0QCdata.showFunctionName);
}

static void
FHZX_teardownAny_xf99(lineNbrT lineNbr)
{
    //TestAidC.getCounts();
}


char*
FHZX_getAssertText_xf99(cfuncNameT testName, lineNbrT lineNbr)
{
    return TestAidQCapi.getAssertText(testName);
}

/**
 * Limit the otherwise super abundant exports.
 */
long oldestUniversalDate = 16400000, newestUniversalDate = 16990000;

FHZ0FilesACdataT FHZ0FilesACdata[] =
{
    {true,          "Adjustments"     },
    {true,          "fs-20200301"     },
    {true,          "fs-20200229"     },
    {true,          "fs-Newbrooke-Mary"     },
    {true,          "fs-myl-core"     },
    {true,          "fs-Millard-Richus-B"     },
    {true,          "fs-Millard-Richus-A"     },
    {true,          "fs-Briscoe-Mar"     },
    {true,          "fs-Millward-Tho"     },
    {false, "famsch-V00732-1"},
    {false, "famsch-V00744-0"},
    {false, "famsch-V01412-8"},
    {false, "famsch-V01412-9"},
    {false, "famsch-V01413-1"},
    {false, "famsch-V01413-5"},
    {false, "famsch-V01414-3"},
    {false, "famsch-V01415-0"},
    {false, "famsch-V01415-1"},
    {false, "famsch-V01415-2"},
    {false, "famsch-V01415-3"},
    {false, "famsch-V01415-7"},
    {false, "famsch-V01416-0-A"},
    {false, "famsch-V01416-0-B"},
    {false, "famsch-V01416-0-C"},
    {false, "famsch-V01416-0-D"},
    {false, "famsch-V01416-0-E"},
    {false, "famsch-V01416-0-F"},
    {true,          "famsch-V01416-0-G"},
    {false, "famsch-V01416-9"},
    {false, "famsch-V01417-2"},
    {false, "famsch-V01418-0"},
    {false, 0}
};


static bool
FHZ0_doSet1_xf99(lineNbrT lineNbr) // Basics
{
    gpSllgChar64PT gp64P = &TestAidZ0QCdata.gp64;
    ErrorWarnCountT ewc = {0, 0, 0, 0};
    
    FHO0ACapi.newFiles(FHZ0_SelSeekFind, gp64P);
    
    if(TestAidC.putTestInts(0, FHZ0control.linePresentingError, __LINE__) != 0 ||
       TestAidC.putTestInts(412, FHZ0control.droppedCount, __LINE__) != 0){
        ewc.classErrors++;
    }
    
    {
        FHO0AC_RPT_HDR
        FHO0AC_WHAT  FHO0AC_TAB
        FHO0AC_PRINM FHO0AC_TAB
        FHO0AC_PRIDB FHO0AC_TAB
        FHO0AC_EVNTT FHO0AC_TAB
        FHO0AC_UVSLD FHO0AC_TAB
        FHO0AC_SCORE FHO0AC_TAB
        if(strcmp(cur.apiP->getField(&cur.data, "u"), "15740415") == 0){
            //int watcher = 0;
        }
        char possiblePeople[] = "smpjo";
        char seekPeople[200] = "";
        int whichPeopleIx = 0;
        for(; possiblePeople[whichPeopleIx]; whichPeopleIx++){
            char* peopleInfo = cur.apiP->getField(&cur.data, possiblePeople + whichPeopleIx);
            if(peopleInfo){
                if(seekPeople[0] != 0){
                    // a comma here would be asthetic but some spreasheets see
                    // it as a column divider given when tab is specified.
                    // Found the problem.
                    strcat(seekPeople, ", ");
                }
                char catType[3] = "./";
                catType[0] = possiblePeople[whichPeopleIx];
                strcat(seekPeople, catType);
                strcat(seekPeople, peopleInfo);
            }
        }
        printf("%s", seekPeople);
        FHO0AC_TAB
        FHO0AC_INDEX FHO0AC_TAB
        FHO0AC_RESRC FHO0AC_TAB
        FHO0AC_RPT_FTR
    }
    
    return ewc.classErrors == 0;
}


static bool
FHZ0_doSet2_xf99(lineNbrT lineNbr) // BatchIdPlace
{
    gpSllgChar64PT gp64P = &TestAidZ0QCdata.gp64;
    ErrorWarnCountT ewc = {0, 0, 0, 0};
    
    FHO0ACapi.newFiles(FHZ0_SelBatchIdPlace, gp64P); // was file #12
    
    {
        char* nP = FHZ0control.buf;
        while(nP < FHZ0control.currWrite){
            char* oP = nP;
            nP += strlen(nP) + 1;
            //if(strstr(oP, "=wFH" FHSEL_BCHIDPLC) != 0){
                printf("%s\n", oP);
            //}
        }
    }
    
    return ewc.classErrors == 0;
}

static bool
FHZ0_doSet3_xf99(lineNbrT lineNbr) // ByRank
{
    gpSllgChar64PT gp64P = &TestAidZ0QCdata.gp64;
    ErrorWarnCountT ewc = {0, 0, 0, 0};
    
    FHO0ACapi.newFiles(FHZ0_SelBirth, gp64P);
    
    if(FHZ0control.linePresentingError == 0)
    {
        char *searched = FHZ0control.currentRead;
        int resultIx = 0;
        int matchCt = 0;
        while(searched < FHZ0control.currWrite)
        {
            char *nextSearched = searched + strlen(searched) + 1;
            
            CursorO0HIthisT thisCur = CursorO0HCapi.newCursor(searched, 0);
            int rankLim = 29;
            int rowRank = 0;
            
            if(((rowRank = CompareHCapi.isSimilar("Ed",
                                                  CursorO0HIapi.getField(&thisCur.data,
                                                                         &FHA_COLTP_PRINM[FHA_LTR_IN_ROW]),
                                                                            FHA_COLTP_SPONM[FHA_TYP_IN_ROW])) > 3 &&
                (rowRank += CompareHCapi.isSimilar("Millward",
                                                   CursorO0HIapi.getField(&thisCur.data,
                                                                          &FHA_COLTP_PRINM[FHA_LTR_IN_ROW]),
                                                                            FHA_COLTP_SPONM[FHA_TYP_IN_ROW])) > 6 &&
                (rowRank += CompareHCapi.isSimilar("Ann",
                                                   CursorO0HIapi.getField(&thisCur.data,
                                                                          &FHA_COLTP_SPONM[FHA_LTR_IN_ROW]),
                                                                            FHA_COLTP_SPONM[FHA_TYP_IN_ROW])) > rankLim &&
                (rowRank += CompareHCapi.isSimilar("Mar",
                                                   CursorO0HIapi.getField(&thisCur.data,
                                                                          &FHA_COLTP_SPONM[FHA_LTR_IN_ROW]),
                                                                            FHA_COLTP_SPONM[FHA_TYP_IN_ROW])) > rankLim &&
                (rowRank += CompareHCapi.isSimilar("El",
                                                   CursorO0HIapi.getField(&thisCur.data,
                                                                          &FHA_COLTP_SPONM[FHA_LTR_IN_ROW]),
                                                                            FHA_COLTP_SPONM[FHA_TYP_IN_ROW])) > rankLim ))
            {
                matchCt++;
            }
            
            searched = nextSearched;
            resultIx++;
        }
        
        if(TestAidC.putTestInts(38, matchCt, __LINE__) != 0){
            TestAidC.getAssertText(__FUNCTION__);
        }
    }
    else
    {
        if(TestAidC.putTestInts(0, FHZ0control.linePresentingError, __LINE__) != 0){
            TestAidC.getAssertText(__FUNCTION__);
        }
    }
    
    return ewc.classErrors == 0;
}

static bool
FHZ0_doSet4_xf99(lineNbrT lineNbr) // ByRank
{
    gpSllgChar64PT gp64P = &TestAidZ0QCdata.gp64;
    ErrorWarnCountT ewc = {0, 0, 0, 0};
    
    FHO0ACapi.newFiles(FHZ0_SelDeath, gp64P);
    
    {
        char* nP = FHZ0control.buf;
        while(nP < FHZ0control.currWrite){
            char* oP = nP;
            nP += strlen(nP) + 1;
            printf("%s\n", oP);
        }
    }
    
    return ewc.classErrors != 0; // true means failure
}

FHZ0QCapiT FHZ0QCapi =
{
    FHZX_setupSuite0_xf99,
    FHZX_isScheduled_xf99,
    FHZX_getAssertText_xf99,
    
    FHZX_setupAny_xf99,
    FHZ0_doSet1_xf99,
    FHZX_teardownAny_xf99,
    
    FHZX_setupAny_xf99,
    FHZ0_doSet2_xf99,
    FHZX_teardownAny_xf99,
    
    FHZX_setupAny_xf99,
    FHZ0_doSet3_xf99,
    FHZX_teardownAny_xf99,
    
    FHZX_setupAny_xf99,
    FHZ0_doSet4_xf99,
    FHZX_teardownAny_xf99,

    FHZX_teardownSuite0_xf99,
};




static bool
FHZ3_doSet1_xf99(lineNbrT lineNbr) // ByRank
{
    return true;
}

static bool
FHZ3_doSet2_xf99(lineNbrT lineNbr) // ByRank
{
    return true;
}

static bool
FHZ3_doSet3_xf99(lineNbrT lineNbr) // ByRank
{
    return true;
}

static bool
FHZ3_doSet4_xf99(lineNbrT lineNbr) // ByRank
{
    return true;
}

FHZ3QCapiT FHZ3QCapi =
{
    FHZX_setupSuite0_xf99,
    FHZX_isScheduled_xf99,
    FHZX_getAssertText_xf99,
    
    FHZX_setupAny_xf99,
    FHZ3_doSet1_xf99,
    FHZX_teardownAny_xf99,
    
    FHZX_setupAny_xf99,
    FHZ3_doSet2_xf99,
    FHZX_teardownAny_xf99,
    
    FHZX_setupAny_xf99,
    FHZ3_doSet3_xf99,
    FHZX_teardownAny_xf99,
    
    FHZX_setupAny_xf99,
    FHZ3_doSet4_xf99,
    FHZX_teardownAny_xf99,

    FHZX_teardownSuite0_xf99,
};
// END FamilyHistoryZ3TestAid.c
/**
 * I decided to do all testing from the Z3
 */
