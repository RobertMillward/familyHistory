//
//  FamilyHistoryPlan.c
//
//  Created by Robert Russell Millward on 2/15/20.
//  Copyright © 2020 Robert Russell Millward. All rights reserved.
//
// os
#include <stdio.h>
// helper and service api's
// data plans
#include "FamilyHistoryPlan.h"
//
#define VALHERE 0

static FamHistDictionaryAndValueT FamHistDictionaryAndValue[] =
{
    {UNI_BCHNBR,    FHA_COL_MTABTCH, VALHERE, {"batch_number",    0}},
    {UNI_FNAME,     FHA_COL_PRINM, VALHERE, {"full_name",       0}},
    {UNI_GNDR,      FHA_COL_PRIGN, VALHERE, {"gender",          0}},
    {UNI_BDT,       FHA_COL_PRIDT, VALHERE, {"birth_date",      0}},
    {UNI_BPLC,      FHA_COL_PRIPL, VALHERE, {"birth_place_text",0}},
    {UNI_CDT,       FHA_COL_PRIDT, VALHERE, {"chr_date",        0}},
    {UNI_CPLC,      FHA_COL_PRIPL, VALHERE, {"chr_place_text",  0}},
    
    {UNI_RESDT,     FHA_COL_PRIDT, VALHERE, {"residence_date",      0}},
    {UNI_RESPLC,    FHA_COL_PRIPL, VALHERE, {"residence_place_text",0}},
    {UNI_MDT,       FHA_COL_PRIDT, VALHERE, {"marriage_date",        0}},
    {UNI_MPLC,      FHA_COL_PRIPL, VALHERE, {"marriage_place_text",  0}},
    
    {UNI_DDT,       FHA_COL_PRIDT, VALHERE, {"death_date",      0}},
    {UNI_DPLC,      FHA_COL_PRIPL, VALHERE, {"death_place_text",0}},
    {UNI_IDT,       FHA_COL_PRIDT, VALHERE, {"burial_date",        0}},
    {UNI_IPLC,      FHA_COL_PRIPL, VALHERE, {"burial_place_text",  0}},
    
    {UNI_FFNM,      FHA_COL_FTHNM, VALHERE, {"father_full_name",   0}},
    {UNI_MFNM,      FHA_COL_MTHNM, VALHERE, {"mother_full_name",0}},
    {UNI_SFNM,      FHA_COL_SPONM, VALHERE, {"spouse_full_name",   0}},
    {UNI_PFNMS,     FHA_COL_OTHPFNM, VALHERE, {"parent_full_names",  0}},
    {UNI_CFNMS,     FHA_COL_OTHCFNM, VALHERE, {"children_full_names",0}},
    {UNI_OFNMS,     "Xlv", VALHERE, {"other_full_names",0}},
    {UNI_OEVENTS,   FHA_COL_OTHEVNT, VALHERE, {"other_events",        0}},
    
    {UNI_SCORE,     "Xlv", VALHERE, {"score",           0}},
    {UNI_IURL,      "Xlv", VALHERE, {"person_url",      0}},
    {UNI_SMT,       FHA_COL_MTSRCTP, VALHERE, {"source_media_type", 0}},
    {UNI_ROLEINREC, FHA_COL_MTRECRL, VALHERE, {"role_in_record",  0}},
    {UNI_RELTOHEAD, FHA_COL_MTRLTHD, VALHERE, {"relationship_to_head", 0}},
    {UNI_SUBCLLID,  "Xlv", VALHERE, {"subcollection_id", 0}},
    {UNI_EUID,      "Xlv", VALHERE, {"easy_unique_id",  0}},
    {UNI_RURL,      "Xlv", VALHERE, {"record_url",      0}},
    {0}
};

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



// END FamilyHistoryPlan.c
/**
 *
 */
