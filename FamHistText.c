//
//  FamHistText.c
//  Repository:Apps/FamilyHist
//
//  Created by Robert R on 2/23/15.
//  Copyright (c) 2015 Robert Russell Millward.
//  All rights reserved.
//
/*
#include <stdio.h>
#include <string.h>
#include "ArchitectureO0.h"
#include "ArchFamilyHistory.h"
#include "FH_FSExportReader.h"
#include "UserKeyboard.h"
#include "Folders4.h"
#include "InitDestroyHelper1.h"
#include "Row2.h"

static struct FamHistTextPrivateS
{
    char *isInit;
    char *fhData;
}fhP =
{
    IDH_DO_HERE_INIT,
    "/Users/robertr1/Box Sync/FamHist" // TECHDEBT
};



static void loadData(FHFSExportReaderP wkP)
{
    char buf[20000];
    manageASCIIrowsT folderInfo =
    {
        buf,
        buf,
        buf,
        buf + sizeof(buf) - 1
    };
    
    FolderC.getFolderInfo(fhP.fhData, "FH_FS*.csv", &folderInfo, 0);
    
    char *mnuFiles[100];
    int mnuIx = 0;
    while(folderInfo.currentRead < folderInfo.currentWrite)
    {
        char *nxtCurRead = folderInfo.currentRead + strlen(folderInfo.currentRead) + 1;
        Row mnuRow = newRow(folderInfo.currentRead, 0);
        mnuFiles[mnuIx++] = mnuRow.getField(&mnuRow, FLDR_NAME);
        folderInfo.currentRead = nxtCurRead;
    }
    mnuFiles[mnuIx] = 0;
    menuChoiceT choice = KeyboardC.getMenuItem(mnuFiles, sizeof(mnuFiles));
    
    char pathInfo[200];
    if(choice > 0)
    {
        sprintf(pathInfo, "%s/%s", fhP.fhData, mnuFiles[choice - 1]);
        FHFSExportReader wkP = newFHFSExportReader(pathInfo);
    }
    
    
}

static char *analyzeMenuLines[] =
{
    "Load Data",
    "Summarize years",
    "Summarize last names",
    "Summarize first names"
};


static void loadNdAnalyzeMenu()
{
    FHFSExportReader wk;
    
    menuChoiceT menuChoice = 99;
    do
    {
        menuChoice = KeyboardC.getMenuItem(analyzeMenuLines, sizeof(analyzeMenuLines));
        switch(menuChoice)
        {
            case 1:
                loadData(&wk);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
        }
    }while(menuChoice != 0);
}

static char *mainMenuLines[] =
{
    "Load and Analyze Data",
    "Find Marriages"
};

static int mainMenu()
{
    menuChoiceT menuChoice = 99;
    do
    {
        menuChoice = KeyboardC.getMenuItem(mainMenuLines, sizeof(mainMenuLines));
        switch(menuChoice)
        {
            case 1:
                loadNdAnalyzeMenu();
                break;
        }
    }while(menuChoice != 0);
    return 0;
}

const FamHistTextX FamHistTextC =
{
    mainMenu
};
 */
//END
