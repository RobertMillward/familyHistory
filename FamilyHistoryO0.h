//
//  FamilyHistoryO0.h
//  2020Mar10Dev
//
//  Created by Robert Russell Millward on 2/15/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
#ifndef FamilyHistoryO0_h
#define FamilyHistoryO0_h
// os
// helper and service api's
// data plans
#include "FamilyHistoryZ0Plan.h"
#include "ArchitectureZ0Plan.h"

typedef struct FHUApplicationClassApiStruct
{
    void (*newFile)(char* path, fileWoTypeT, FHZ0ReportsT, gpSllgChar64PT);
}FHO0ACapiT;

extern FHO0ACapiT FHO0ACapi;

#define FHO0AC_RPT_HDR \
char* nP = FHZ0control.buf; \
while(nP < FHZ0control.currWrite){ \
    char* oP = nP; \
    nP += strlen(nP) + 1; \
    CursorO0HIthisT cur = CursorO0HCapi.newCursor(oP, gp64P);

#define FHO0AC_WHAT  printf("%s", cur.apiP->getField(&cur.data, "w"));
#define FHO0AC_TAB   printf("\t");
#define FHO0AC_SCORE printf("%s", cur.apiP->getField(&cur.data, FHA_COLTP_SCORE + FHA_LTR_IN_ROW));
#define FHO0AC_PRINM printf("%s", cur.apiP->getField(&cur.data, FHA_COLTP_PRINM + FHA_LTR_IN_ROW));

#define FHO0AC_PRIDB printf("%s", cur.apiP->getField(&cur.data, FHA_COLTP_PRIDB + FHA_LTR_IN_ROW));
#define FHO0AC_EVNTT printf("%s", cur.apiP->getField(&cur.data, FHA_COLTP_EVNTT + FHA_LTR_IN_ROW));
#define FHO0AC_UVSLD printf("%s", cur.apiP->getField(&cur.data, FHA_COLTP_UVSLDT + FHA_LTR_IN_ROW));

#define FHO0AC_INDEX printf("%s", cur.apiP->getField(&cur.data, FHA_COLTP_PRVDID + FHA_LTR_IN_ROW));
#define FHO0AC_RESRC printf("%s", cur.apiP->getField(&cur.data, FHA_COLTP_BTHID + FHA_LTR_IN_ROW));

#define FHO0AC_RPT_FTR \
        printf("\n");\
    }

#endif /* FamilyHistoryO0_h */
/**
 *
 */
