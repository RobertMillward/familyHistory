//
//  FamHistZ3XCTests.m
//  2020Mar10Dev
//
//  Created by Robert R on 2/17/15.
//  Copyright (c) 2020 Robert Russell Millward. All rights reserved.
//
// os
#import <XCTest/XCTest.h>
// quality
#import "FamilyHistoryZ3TestAid.h"

@interface FamHistO0Tests : XCTestCase

@end

@interface FamHistO3Tests : XCTestCase

@end







@implementation FamHistO0Tests

- (void)setUp {
    FHZ3QCapi.setupParseSet1(__LINE__);
}

- (void)tearDown {
    FHZ3QCapi.teardownParseSet1(__LINE__);
}


- (void)test2015SampleNotWorking {
    if(FHZ3QCapi.isScheduled(__FUNCTION__, __LINE__)){
        if(FHZ3QCapi.doSet4(__LINE__)){
            XCTAssert(NO, @"%@", [[NSString alloc] initWithUTF8String:FHZ3QCapi.getAssertText(__FUNCTION__, __LINE__)]);
        }
    }
}




- (void)test2020Basics
{
    
}// END Basics

- (void)test2020BatchIdPlace
{
    
    
    
}// END BatchIdLoc


- (void)testAselectByRank
{
    
}


@end

@implementation FamHistO3Tests

@end


// END FamHistZ3XCTests.m
/**
 *
 */
