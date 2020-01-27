//
//  FamHistTests.m
//
//  Created by Robert R on 2/17/15.
//  Copyright (c) 2015 Robert Russell Millward. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
#import "ArchFamilyHistory.h"
#import "FH_FSExportReader.h"
#import "RowO2.h"
#import "TestAidO0.h"

@interface FamHistTests : XCTestCase

@end

@implementation FamHistTests

- (void)setUp {
}

- (void)tearDown {
}


char *edw = "Downloads/FAMHIST/dev/FHHAGUEBENJAMIN.csv";

- (void)testAfhxr
{
    FHFSExportReader fsEx = newFHFSExportReader(edw);
    
    if(TestAidC.putTestInts(0, fsEx.linePresentingError, __LINE__) != 0){
        TestAidC.getAssertText(__FUNCTION__);
    }
    
    if(TestAidC.putTestInts(270, fsEx.droppedCount, __LINE__) != 0){
        TestAidC.getAssertText(__FUNCTION__);
    }
}


- (void)testAselectByRank
{
    FHFSExportReader fsEx = newFHFSExportReader(edw);
    
    if(fsEx.linePresentingError == 0)
    {
        
        char *searched = fsEx.currentRead;
        int resultIx = 0;
        int matchCt = 0;
        while(searched < fsEx.currWrite)
        {
            char *nextSearched = searched + strlen(searched) + 1;
            
            Row thisRow = newRow(searched, 0);
            int rankLim = 29;
            int rowRank = 0;
            if(((rowRank  = thisRow.isSimilar(&thisRow, "Ed",  FHA_COL_PRINM[FHA_COL_IN_ROW])) > 3 &&
                (rowRank += thisRow.isSimilar(&thisRow, "Millward", FHA_COL_PRINM[FHA_COL_IN_ROW])) > 6 &&
                ((rowRank += thisRow.isSimilar(&thisRow, "Ann", FHA_COL_SPONM[FHA_COL_IN_ROW])) > rankLim ||
                 (rowRank += thisRow.isSimilar(&thisRow, "Mar", FHA_COL_SPONM[FHA_COL_IN_ROW])) > rankLim ||
                 (rowRank += thisRow.isSimilar(&thisRow, "El",  FHA_COL_SPONM[FHA_COL_IN_ROW])) > rankLim)))
            {
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
        if(TestAidC.putTestInts(0, fsEx.linePresentingError, __LINE__) != 0){
            TestAidC.getAssertText(__FUNCTION__);
        }
    }
}


//- (void)testPerformanceExample {
//    // This is an example of a performance test case.
//    [self measureBlock:^{
//        // Put the code you want to measure the time of here.
//    }];
//}

@end
