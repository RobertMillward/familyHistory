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
    FHZ0QCapi.setupSet1(__LINE__);
}

- (void)tearDown {
    FHZ0QCapi.teardownSet1(__LINE__);
}


- (void)test2020SeekFind
{
    if(FHZ0QCapi.isScheduled(__FUNCTION__, __LINE__)){
        if(FHZ0QCapi.doSet1(__LINE__)){
            XCTAssert(NO, @"%@", [[NSString alloc] initWithUTF8String:FHZ3QCapi.getAssertText(__FUNCTION__, __LINE__)]);
        }
    }
}// END Basics

- (void)test2020BatchIdPlace
{
    if(FHZ0QCapi.isScheduled(__FUNCTION__, __LINE__)){
        if(FHZ0QCapi.doSet2(__LINE__)){
            XCTAssert(NO, @"%@", [[NSString alloc] initWithUTF8String:FHZ3QCapi.getAssertText(__FUNCTION__, __LINE__)]);
        }
    }
}// END BatchIdPlace


- (void)test2020SelectByRank
{
    if(FHZ0QCapi.isScheduled(__FUNCTION__, __LINE__)){
        if(FHZ0QCapi.doSet3(__LINE__)){
            XCTAssert(NO, @"%@", [[NSString alloc] initWithUTF8String:FHZ3QCapi.getAssertText(__FUNCTION__, __LINE__)]);
        }
    }
}// END selectByRank


- (void)test2020SelectBirthdays
{
    if(FHZ0QCapi.isScheduled(__FUNCTION__, __LINE__)){
        if(FHZ0QCapi.doSet4(__LINE__)){
            XCTAssert(NO, @"%@", [[NSString alloc] initWithUTF8String:FHZ3QCapi.getAssertText(__FUNCTION__, __LINE__)]);
        }
    }
}// END selectBirthdays

@end

@implementation FamHistO3Tests


- (void)setUp {
    FHZ3QCapi.setupSet1(__LINE__);
}

- (void)tearDown {
    FHZ3QCapi.teardownSet1(__LINE__);
}

@end


// END FamHistZ3XCTests.m
/**
 *
 */
