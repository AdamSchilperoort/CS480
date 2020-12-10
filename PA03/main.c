#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "booleans.h"
#include "pcb.h"
#include "errors.h"
#include "dataUpload.h"
#include "logFileUtils.h"
#include "simulator.h"
#include "strUtils.h"
#include "simtimer.h"

int main(int argc, char const *argv[])
{

    //VARIABLE DECLARATIONS
    char *fileExt;
    char *fileName = (char *)argv[1];
    struct configValues *settings = (struct configValues *)calloc(1, sizeof(struct configValues));
    struct simAction *actionsHead = (struct simAction *)calloc(1, sizeof(struct simAction));
    struct logEvent *logList = (struct logEvent *)calloc(1, sizeof(struct logEvent));
    int cfgVal, mdfVal, verificationVal, simVal;
    bool logToFile = FALSE;
    bool logToMon = FALSE;

    printf("\n\nSimulator Program\n==============================\n\n");

    //Check for correct number of arguments
    if (argc != 2)
    {
        displayError(NUM_ARGS_ERROR);
        return 1;
    }

    //check correct file format
    fileExt = getFileExt(fileName);
    if (!strCmp(fileExt, ".cnf"))
    {
        displayError(FILE_TYPE_ERROR);
        return 1;
    }

    //freeing file extension memory because it is no longer needed
    free(fileExt);

    //STARTING FILE UPLOAD PROCESS
    printf("Uploading Configuration File: %s \n==============================\n\n", fileName);

    //READ IN CONFIG FILE VALUES
    cfgVal = readConfigFile(fileName, settings);

    //ERROR CHECKING FOR CONFIG FILE READING
    if (cfgVal > 0)
    {
        //if log to was set before error received
        if (settings->logTo)
        {
            //create log file if necessary
            if (strCmp(settings->logTo, "Both") || strCmp(settings->logTo, "File"))
            {
                createLogFile(settings->logPath, logList);
            }
        }

        //displays correct error code and returns 1
        displayError(cfgVal);
        return 1;
    }
    
    verificationVal = verifySettings(settings);

    if (verificationVal)
    {
        displayError(verificationVal);

        return 1;
    }

    //sets logToFile flag if "Both" or "File" are chosen
    if (strCmp(settings->logTo, "Both") || strCmp(settings->logTo, "File"))
    {
        logToFile = TRUE;
    }

    //sets logToMon flag if "Both" or "Monitor" are chosen
    if (strCmp(settings->logTo, "Both") || strCmp(settings->logTo, "Monitor"))
    {
        logToMon = TRUE;
    }

    //PRINTING CONFIG FILE VALUES
    if (logToMon)
    {
        printConfigValues(settings, fileName);
    }

    if (logToFile)
    {
        createLogHeader(logList);
        appendSettingsToLog(logList, settings);
    }

    //BEGINNING MDF FILE UPLOAD
    printf("Uploading Meta Data file: %s\n==============================\n\n", settings->mdfPath);

    mdfVal = readMetaDataFile(settings->mdfPath, actionsHead);

    //ERROR CHECKING FOR META-DATA FILE READING
    if (mdfVal > 0)
    {
        if (logToFile)
        {
            createLogFile(settings->logPath, logList);
        }
        displayError(mdfVal);
    }

    if (logToMon)
    {
        printSimActions(actionsHead);
    }

    //verifies simulator actions
    verificationVal = verifySimActions(actionsHead);

    //error checking for verification of sim acitons
    if (verificationVal > 0)
    {

        //creates log file if necessary
        if (logToFile)
        {
            createLogFile(settings->logPath, logList);
        }

        //displays appropriate error message
        displayError(verificationVal);
    }

    //simulates OS/Actions
    simVal = simulate(actionsHead, settings, logList);

    //error checking for sim values, negative values because scheduler returns positives for PCB array positions
    if (simVal < 0)
    {

        if (logToFile)
        {
            createLogFile(settings->logPath, logList);
        }

        displayError(simVal);

        return 1;
    }

    //if logToFile is set, creates a log file
    if (logToFile)
    {
        createLogFile(settings->logPath, logList);
    }

    //FREEING DATA STRUCTS USED TO STORE READ INFORMATION
    freeActions(actionsHead);
    freeLog(logList);
    freeConfigValues(settings);

    return 0;
}

