//
//  FamHistZ0XCTests.m
//  2020Mar10Dev
//
//  Created by Robert R on 2/17/15.
//  Copyright (c) 2020 Robert Russell Millward. All rights reserved.
//
// os
//#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
// helper and service api's
#import "CompareO0.h"
#import "CursorO0.h"
//#import "RowO0.h"
#import "FamilyHistoryO0.h"
// data plans
#import "ArchFamilyHistory.h"
#import "FamilyHistoryZ0Plan.h"
// quality
#import "TestAidZ0.h"

@interface FamHistTests : XCTestCase

@end

@implementation FamHistTests

- (void)setUp {
}

- (void)tearDown {
}


char *edw[] = {
    "fsmyl-core",
    "famsch-V00732-1",
    "famsch-V00744-0",
    "famsch-V01412-8",
    "famsch-V01412-9",
    "famsch-V01413-1",
    "famsch-V01413-5",
    "famsch-V01414-3"
    "famsch-V01415-0",
    "famsch-V01415-1",
    "famsch-V01415-2",
    "famsch-V01415-3",
    "famsch-V01415-7",
    "famsch-V01416-0-A",
    "famsch-V01416-9",
    "famsch-V01417-2",
    "famsch-V01418-0",
    0
};

- (void)test2020Basics
{
    gpSllgChar64PT gp64P = &TestAidZ0QCdata.gp64;
    
    FHO0ACapi.newFile(INIT_DB_PATH, edw[0], FHZ0_SeekFindRpt, gp64P);
    
    if(TestAidC.putTestInts(0, FHZ0control.linePresentingError, __LINE__) != 0){
        TestAidC.getAssertText(__FUNCTION__);
    }
    
    if(TestAidC.putTestInts(412, FHZ0control.droppedCount, __LINE__) != 0){
        TestAidC.getAssertText(__FUNCTION__);
    }
    
    {
        char* nP = FHZ0control.buf;
        while(nP < FHZ0control.currWrite){
            char* oP = nP;
            nP += strlen(nP) + 1;
            if(strstr(oP, "=wFHBatchId") != 0){
                printf("%s\n", oP);
            }
        }
    }
    
    {
        FHO0AC_RPT_HDR
        FHO0AC_WHAT  FHO0AC_TAB
        FHO0AC_PRINM FHO0AC_TAB
        FHO0AC_PRIDB FHO0AC_TAB
        FHO0AC_EVNTT FHO0AC_TAB
        FHO0AC_UVSLD FHO0AC_TAB
        FHO0AC_SCORE FHO0AC_TAB
            char possiblePeople[] = "smpo";
            char seekPeople[100] = "";
            int whichPeopleIx = 0;
            for(; possiblePeople[whichPeopleIx]; whichPeopleIx++){
                char* peopleInfo = cur.apiP->getField(&cur.data, possiblePeople + whichPeopleIx);
                if(peopleInfo){
                    if(seekPeople[0] != 0){
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
}// END Basics

- (void)test2020BatchIdLoc
{
    gpSllgChar64PT gp64P = &TestAidZ0QCdata.gp64;
    
    FHO0ACapi.newFile(INIT_DB_PATH, edw[12], FHZ0_BatchIdLocRpt, gp64P);
    
    {
        char* nP = FHZ0control.buf;
        while(nP < FHZ0control.currWrite){
            char* oP = nP;
            nP += strlen(nP) + 1;
            if(strstr(oP, "=wFHBatchIdLoc") != 0){
                printf("%s\n", oP);
            }
        }
    }
    
    
}// END BatchIdLoc


- (void)testAselectByRank
{
    FHO0ACapi.newFile(INIT_DB_PATH, edw[0], FHZ0_BirthRpt, &TestAidZ0QCdata.gp64);
    
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
}


@end
// END FamHistZ0XCTests.m
/**
 *
 */
