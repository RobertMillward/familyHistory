//
//  ArchFamilyHistory.h
//
//  Created by Robert R on 2/9/15.
//  Copyright (c) 2015 Robert Russell Millward.
//  All rights reserved.
//

#ifndef FamilyHist_ArchFamilyHistory_h
#define FamilyHist_ArchFamilyHistory_h

#define FHGROUP "FAMHIST"



typedef const struct FamHistTextS
{
    int (*mainMenu)(void);
}FamHistTextX, *FamHistTextXP;

extern FamHistTextX FamHistTextC;


#endif
