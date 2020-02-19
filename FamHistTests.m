//
//  FamHistTests.m
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
#import "FamilyHistoryApi.h"
// data plans
#import "ArchFamilyHistory.h"
#import "FamilyHistoryPlan.h"
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
    "famschV00732-1",
    "famschV00744-0",
    "famschV01412-8",
    "famschV01412-9",
    "famschV01413-1",
    "famschV01413-5",
    "famschV01414-3"
    "famschV01415-0",
    "famschV01415-1",
    "famschV01415-2",
    "famschV01415-3",
    "famschV01415-7",
    "famschV01416-0",
    "famschV01416-9",
    "famschV01417-2",
    "famschV01418-0",
    0
};

- (void)test2020Basics
{
    FHU_HCapi.newFile(INIT_DB_PATH, edw[0], &TestAidZ0QCdata.gp64);
    
    if(TestAidC.putTestInts(0, FHU_control.linePresentingError, __LINE__) != 0){
        TestAidC.getAssertText(__FUNCTION__);
    }
    
    if(TestAidC.putTestInts(412, FHU_control.droppedCount, __LINE__) != 0){
        TestAidC.getAssertText(__FUNCTION__);
    }
    char* oP = FHU_control.buf;
    while(oP < FHU_control.currWrite){
        if(strstr(oP, "=wFHBatchId") != 0){
            printf("%s\n", oP);
        }
        oP += strlen(oP) + 1;
    }
    
    oP = FHU_control.buf;
    while(oP < FHU_control.currWrite){
        if(strstr(oP, "=wFHNmDtBch") != 0){
            printf("%s\n", oP);
        }
        oP += strlen(oP) + 1;
    }

    oP = FHU_control.buf;
    while(oP < FHU_control.currWrite){
        if(strstr(oP, "=wFHBatchId") == 0 && strstr(oP, "=wFHNmDtBch") == 0){
            printf("%s\n", oP);
        }
        oP += strlen(oP) + 1;
    }
}


- (void)testAselectByRank
{
    FHU_HCapi.newFile(INIT_DB_PATH, edw[0], &TestAidZ0QCdata.gp64);
    
    if(FHU_control.linePresentingError == 0)
    {
        char *searched = FHU_control.currentRead;
        int resultIx = 0;
        int matchCt = 0;
        while(searched < FHU_control.currWrite)
        {
            char *nextSearched = searched + strlen(searched) + 1;
            
            CursorO0HIthisT thisCur = CursorO0HCapi.newCursor(searched, 0);
            int rankLim = 29;
            int rowRank = 0;
            
            if(((rowRank = CompareHCapi.isSimilar("Ed",
                                                  CursorO0HIapi.getField(&thisCur.data, &FHA_COL_PRINM[FHA_COL_IN_ROW]),
                                                  FHA_COL_SPONM[FHA_TYP_IN_ROW])) > 3 &&
                (rowRank += CompareHCapi.isSimilar("Millward",
                                                  CursorO0HIapi.getField(&thisCur.data, &FHA_COL_PRINM[FHA_COL_IN_ROW]),
                                                  FHA_COL_SPONM[FHA_TYP_IN_ROW])) > 6 &&
                (rowRank += CompareHCapi.isSimilar("Ann",
                                                  CursorO0HIapi.getField(&thisCur.data, &FHA_COL_SPONM[FHA_COL_IN_ROW]),
                                                  FHA_COL_SPONM[FHA_TYP_IN_ROW])) > rankLim &&
                (rowRank += CompareHCapi.isSimilar("Mar",
                                                  CursorO0HIapi.getField(&thisCur.data, &FHA_COL_SPONM[FHA_COL_IN_ROW]),
                                                  FHA_COL_SPONM[FHA_TYP_IN_ROW])) > rankLim &&
                (rowRank += CompareHCapi.isSimilar("El",
                                                   CursorO0HIapi.getField(&thisCur.data, &FHA_COL_SPONM[FHA_COL_IN_ROW]),
                                                   FHA_COL_SPONM[FHA_TYP_IN_ROW])) > rankLim )){
//            if(((rowRank  = thisCur.apiP->isSimilar(&thisCur.data, "Ed",  FHA_COL_PRINM[FHA_COL_IN_ROW])) > 3 &&
//                (rowRank += thisCur.apiP->isSimilar(&thisCur.data, "Millward", FHA_COL_PRINM[FHA_COL_IN_ROW])) > 6 &&
//                ((rowRank += thisCur.apiP->isSimilar(&thisCur.data, "Ann", FHA_COL_SPONM[FHA_COL_IN_ROW])) > rankLim ||
//                 (rowRank += thisCur.apiP->isSimilar(&thisCur.data, "Mar", FHA_COL_SPONM[FHA_COL_IN_ROW])) > rankLim ||
//                 (rowRank += thisCur.apiP->isSimilar(&thisCur.data, "El",  FHA_COL_SPONM[FHA_COL_IN_ROW])) > rankLim)))
//            {
                /*
                 char wk[200];
                 thisRow.toASCII(&thisRow, wk);
                 printf("%4i [%4d] %3d '%s'\n", __LINE__, resultIx, rowRank, wk);
                 */
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
        if(TestAidC.putTestInts(0, FHU_control.linePresentingError, __LINE__) != 0){
            TestAidC.getAssertText(__FUNCTION__);
        }
    }
}


@end
// END FamHistTests.m
/**
 *
 */
