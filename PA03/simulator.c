#include "pcb.h"
#include "simtimer.h"
#include "scheduler.h"
#include "booleans.h"
#include "strUtils.h"
#include "logFileUtils.h"
#include "errors.h"
#include "memoryManagement.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/*
    * simulates an operating system and the actions that run on it:
        * inputs are a simAction linked list, configValues struct, and a logEvent linked list
        * starts OS
        * builds array of PCBs
        * schedules next application to be run
        * runs applications
        * returns 0 when finished running
*/
int simulate(struct simAction *actionsList, struct configValues *settings, struct logEvent *logList)
{
    int numApps, runningApp, timeSec, timeUsec, totalTime, cycleTime, memoryReturn;
    int memoryValues[3];
    char *type, *line;
    struct PCB *controlBlock;
    struct MMU *mmu = calloc(1, sizeof(struct MMU *));
    struct simAction *programCounter;
    struct timeval startTime;
    struct timeval runtime;
    bool programsToRun = TRUE;
    bool logToMon = FALSE;
    bool logToFile = FALSE;

    //identifying that this mmu link is the first mmu unit
    mmu->id = FIRST;

    //allocating memory for line
    line = calloc(100, sizeof(char));

    //settings flags for logging
    if (strCmp(settings->logTo, "Both"))
    {
        logToMon = TRUE;
        logToFile = TRUE;
    }
    else if (strCmp(settings->logTo, "Monitor"))
    {
        logToMon = TRUE;
    }
    else if (strCmp(settings->logTo, "File"))
    {
        logToFile = TRUE;
    }

    logIt("\n\nBegin Simulation\n==============================\n\n", logList, logToMon, logToFile);

    //getting start time of OS
    gettimeofday(&startTime, NULL);

    sprintf(line, "[%lf] OS: System Start\n", (double)0);
    logIt(line, logList, logToMon, logToFile);

    sprintf(line, "[%lf] OS: Creating Process Control Blocks\n", tv2double(execTime(startTime)));
    logIt(line, logList, logToMon, logToFile);

    //counting number of applications in actions list
    numApps = countApplications(actionsList);

    //allocating memory for PCB array
    struct PCB *pcbList[numApps];

    //creates PCB array
    createPCBList(pcbList, actionsList, settings);

    sprintf(line, "[%lf] OS: All processes initialized in \"new\" state\n", tv2double(execTime(startTime)));
    logIt(line, logList, logToMon, logToFile);

    //setting state to ready for all PCBs in PCB array
    setStatesReady(pcbList, numApps);

    sprintf(line, "[%lf] OS: All processes now set to state \"ready\"\n", tv2double(execTime(startTime)));
    logIt(line, logList, logToMon, logToFile);

    //schedules the next app to be run
    runningApp = scheduleNext(pcbList, settings->cpuSched, numApps);

    //error checking
    if (runningApp < 0)
    {
        return runningApp;
    }

    sprintf(line, "[%lf] OS: Selected process %d with %dms remaining\n\n", tv2double(execTime(startTime)), runningApp, pcbList[runningApp]->timeRemaining);
    logIt(line, logList, logToMon, logToFile);

    while (programsToRun)
    {

        //sets current PCB and current program counter
        controlBlock = pcbList[runningApp];
        programCounter = controlBlock->pc;

        //iterates until A(end) occurs
        while (!strCmp(programCounter->operationString, "end") && controlBlock->timeRemaining != 0)
        {
            memoryReturn = 0;

            //if program counter's command letter is not 'A'
            if (programCounter->commandLetter != 'A')
            {

                switch (programCounter->commandLetter)
                {
                //if program counter's command letter is 'P
                case 'P':
                    cycleTime = settings->cpuCycleTime;
                    type = "operation";
                    break;
                //if program counter's command letter is 'I'
                case 'I':
                    cycleTime = settings->ioCycleTime;
                    type = "input";
                    break;
                //if program counter's command letter is 'O'
                case 'O':
                    cycleTime = settings->ioCycleTime;
                    type = "output";
                    break;
                //if program counter's command letter is 'M'
                case 'M':
                    cycleTime = settings->ioCycleTime;
                    type = "memory";
                    break;
                //should never be reached by this point in the program
                default:
                    break;
                }

                //creating time time values and setting timeval data
                totalTime = programCounter->assocVal * cycleTime;
                timeSec = totalTime / MS_PER_SEC;
                timeUsec = (totalTime % MS_PER_SEC) * USEC_PER_MS;
                runtime.tv_sec = timeSec;
                runtime.tv_usec = timeUsec;

                if (programCounter->commandLetter == 'M')
                {

                    stripMemoryValues(programCounter->assocVal, memoryValues);

                    if (strCmp(programCounter->operationString, "allocate"))
                    {
                        sprintf(line, "[%lf] Process: %d, MMU attempt to allocate %d/%d/%d\n", tv2double(execTime(startTime)), controlBlock->processNum, memoryValues[0], memoryValues[1], memoryValues[2]);
                        logIt(line, logList, logToMon, logToFile);

                        memoryReturn = allocate(mmu, memoryValues[0], memoryValues[1], memoryValues[2], settings->memoryAvailable, controlBlock);

                        if (memoryReturn)
                        {
                            if (memoryReturn == MEMORY_ALREADY_ALLOCATED_ERROR)
                            {
                                sprintf(line, "[%lf] Process: %d, MMU failed to allocate. Memory already allocated. \n\n", tv2double(execTime(startTime)), controlBlock->processNum);
                                logIt(line, logList, logToMon, logToFile);
                                controlBlock->timeRemaining = 0;
                            }
                            else if (memoryReturn == CANNOT_ALLOCATE_MEMORY_AMOUNT_ERROR)
                            {
                                sprintf(line, "[%lf] Process: %d, MMU failed to allocate. System memory full. \n\n", tv2double(execTime(startTime)), controlBlock->processNum);
                                logIt(line, logList, logToMon, logToFile);
                                controlBlock->timeRemaining = 0;
                            }
                        }
                        else
                        {
                            sprintf(line, "[%lf] Process: %d, MMU successful allocate %d/%d/%d\n", tv2double(execTime(startTime)), controlBlock->processNum, memoryValues[0], memoryValues[1], memoryValues[2]);
                            logIt(line, logList, logToMon, logToFile);
                        }
                    }
                    else if (strCmp(programCounter->operationString, "access"))
                    {
                        sprintf(line, "[%lf] Process: %d, MMU attempt to access %d/%d/%d\n", tv2double(execTime(startTime)), controlBlock->processNum, memoryValues[0], memoryValues[1], memoryValues[2]);
                        logIt(line, logList, logToMon, logToFile);

                        memoryReturn = access(mmu, controlBlock->processNum, memoryValues[0], memoryValues[1], memoryValues[2]);

                        if (memoryReturn)
                        {
                            if (memoryReturn == MEMORY_ACCESS_OUTSIDE_BOUNDS_ERROR)
                            {
                                sprintf(line, "[%lf] Process: %d, MMU failed to access. Outside allocated memory bounds. \n\n", tv2double(execTime(startTime)), controlBlock->processNum);
                            }
                            else if (memoryReturn == WRONG_MEMORY_ACCESS_ID_ERROR)
                            {
                                sprintf(line, "[%lf] Process: %d, MMU failed to access. Incorrect ID. \n\n", tv2double(execTime(startTime)), controlBlock->processNum);
                            }
                            else if (memoryReturn == CANNOT_ACCESS_MEMORY_ERROR)
                            {
                                sprintf(line, "[%lf] Process: %d, MMU failed to access. Base not allocated.\n\n", tv2double(execTime(startTime)), controlBlock->processNum);
                            }
                            else if (memoryReturn == PROCESS_DOES_NOT_OWN_MEMORY)
                            {
                                sprintf(line, "[%lf] Process: %d, MMU failed to access. Process does not own memory being tried to be accessed.\n\n", tv2double(execTime(startTime)), controlBlock->processNum);
                            }
                            logIt(line, logList, logToMon, logToFile);
                            controlBlock->timeRemaining = 0;
                        }
                        else
                        {
                            sprintf(line, "[%lf] Process: %d, MMU successful access %d/%d/%d\n", tv2double(execTime(startTime)), controlBlock->processNum, memoryValues[0], memoryValues[1], memoryValues[2]);
                            logIt(line, logList, logToMon, logToFile);
                        }
                    }
                }
                else
                {
                    sprintf(line, "[%lf] Process: %d, %s %s start\n", tv2double(execTime(startTime)), controlBlock->processNum, programCounter->operationString, type);
                    logIt(line, logList, logToMon, logToFile);

                    //runs app for amount of time stored in runtime struct
                    runFor(runtime);

                    //subtracts from timeRemaining how much time the app was run for
                    controlBlock->timeRemaining -= ((timeSec * MS_PER_SEC) + (timeUsec / USEC_PER_MS));

                    if (controlBlock->timeRemaining == 0)
                    {
                        sprintf(line, "[%lf] Process: %d, %s %s end\n\n", tv2double(execTime(startTime)), controlBlock->processNum, programCounter->operationString, type);
                        logIt(line, logList, logToMon, logToFile);
                    }
                    else
                    {
                        sprintf(line, "[%lf] Process: %d, %s %s end\n", tv2double(execTime(startTime)), controlBlock->processNum, programCounter->operationString, type);
                        logIt(line, logList, logToMon, logToFile);
                    }
                }
            }

            //iterates program counter
            programCounter = programCounter->next;
        }

        //if timeRemaining is zero logic
        if (controlBlock->timeRemaining == 0)
        {

            //sets state to exit for control block
            controlBlock->state = "exit";

            if (memoryReturn)
            {
                sprintf(line, "[%lf] OS: Process %d experiences segmentation fault.\n", tv2double(execTime(startTime)), controlBlock->processNum);
                logIt(line, logList, logToMon, logToFile);
            }

            sprintf(line, "[%lf] OS: Process %d ended and set in \"exit\" state\n", tv2double(execTime(startTime)), controlBlock->processNum);
            logIt(line, logList, logToMon, logToFile);
        }

        //schedules the next app to be run
        runningApp = scheduleNext(pcbList, settings->cpuSched, numApps);

        //if ALL_PROGRAMS_DONE received from scheduler, set programsToRun to FALSE
        if (runningApp == ALL_PROGRAMS_DONE)
        {
            programsToRun = FALSE;
        }
        else
        {
            sprintf(line, "[%lf] OS: Selected process %d with %dms remaining\n", tv2double(execTime(startTime)), runningApp, pcbList[runningApp]->timeRemaining);
            logIt(line, logList, logToMon, logToFile);
            pcbList[runningApp]->state = "running";
            sprintf(line, "[%lf] OS: Process %d set in RUNNING state\n\n", tv2double(execTime(startTime)), runningApp);
            logIt(line, logList, logToMon, logToFile);
        }
    }

    sprintf(line, "[%lf] OS: System end\n\n", tv2double(execTime(startTime)));
    logIt(line, logList, logToMon, logToFile);

    logIt(line, logList, logToMon, logToFile);

    freePCBs(pcbList, numApps);
    free(line);
    
    printf("End Simulation - Complete\n==============================\n\n");

    return 0;
}

