#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "logFileUtils.h"
#include "errors.h"
#include "strUtils.h"
#include "pcb.h"
#include "booleans.h"

/*
    appends event to log:
        * iterates over log until the end
        * creates a new log entry at the end of the list
*/
void appendToLog(struct logEvent *logList, char *entry)
{
    struct logEvent *logIter = logList;
    int pos = 0;

    //iterating over log
    while (logIter->next)
    {
        pos++;
        logIter = logIter->next;
    }

    //if log entry exists already, allocate memory for next and set logIter to the next node in linked list
    if (logIter->entry)
    {
        logIter->next = (struct logEvent *)calloc(1, sizeof(struct logEvent));
        logIter = logIter->next;
    }

    //allocating memory for and setting log entry data
    logIter->entry = (char *)calloc(strLen(entry) + 1, sizeof(char));
    strCopy(entry, logIter->entry);
}

/*
    creates the log header information
*/
void createLogHeader(struct logEvent *logList)
{
    appendToLog(logList, "==================================================\n");
    appendToLog(logList, "Simulator Log File Header\n");
    appendToLog(logList, "==================================================\n\n");
}

/*
    appends configValues struct to log:
        * allocates all temporary storage strings
        * puts temporary strings into an array for iteration
        * iterates array and adds values to log
*/
void appendSettingsToLog(struct logEvent *logList, struct configValues *settings)
{
    //declaring and allocating memory for all relevant vars
    char *version, *program, *cpuSched, *quantum, *memAvail, *cpuCycle, *ioCycle, *logTo, *logPath;
    version = calloc(100, sizeof(char));
    program = calloc(100, sizeof(char));
    cpuSched = calloc(100, sizeof(char));
    quantum = calloc(100, sizeof(char));
    memAvail = calloc(100, sizeof(char));
    cpuCycle = calloc(100, sizeof(char));
    ioCycle = calloc(100, sizeof(char));
    logTo = calloc(100, sizeof(char));
    logPath = calloc(100, sizeof(char));
    char *all[9] = {version, program, cpuSched, quantum, memAvail, cpuCycle, ioCycle, logTo, logPath};
    int pos;

    //setting the string contents for all temporary storage strings
    sprintf(version, "Version                : %f\n", settings->ver);
    sprintf(program, "Program file name      : %s\n", settings->mdfPath);
    sprintf(cpuSched, "CPU schedule selection : %s\n", settings->cpuSched);
    sprintf(quantum, "Quantum time           : %d\n", settings->quantumTime);
    sprintf(memAvail, "Memory Available       : %d\n", settings->memoryAvailable);
    sprintf(cpuCycle, "Process cycle rate     : %d\n", settings->cpuCycleTime);
    sprintf(ioCycle, "I/O cycle rate         : %d\n", settings->ioCycleTime);
    sprintf(logTo, "Log to selection       : %s\n", settings->logTo);
    sprintf(logPath, "Log file name          : %s\n\n", settings->logPath);

    //iterating over array of temporary storage strings and appending them to the log
    for (pos = 0; pos < 9; pos++)
    {
        appendToLog(logList, all[pos]);
        free(all[pos]);
    }

    //appends a section to the log to separate it from other sections for easier viewing
    appendToLog(logList, "==================================================\n");
    appendToLog(logList, "==================================================\n\n");
}

/* 
    iterates over simAction struct linked list and appends data to log:
        * iterates over sim actions
        * appends all their data to the log
*/
void appendSimActionsToLog(struct logEvent *logList, struct simAction *head)
{
    struct simAction *actionsIter = head;
    char *cmd, *opString, *assocVal;

    //iterating over linked list
    while (actionsIter->next)
    {
        //allocating memory for each temporary storage string
        cmd = calloc(100, sizeof(char));
        opString = calloc(100, sizeof(char));
        assocVal = calloc(100, sizeof(char));

        //setting each temporary storage string
        sprintf(cmd, "Op code letter: %c\n", actionsIter->commandLetter);
        sprintf(opString, "Op code name  : %s\n", actionsIter->operationString);
        sprintf(assocVal, "Op code value : %ld\n\n", actionsIter->assocVal);

        //adding each temporary storage string to the log
        appendToLog(logList, cmd);
        appendToLog(logList, opString);
        appendToLog(logList, assocVal);

        actionsIter = actionsIter->next;
    }
}

/*
    creates a log file using a file name and log linked list:
        * opens file
        * iterates over log
        * prints log events to file
        * returns 0 on success, error code otherwise
*/
int createLogFile(char *fileName, struct logEvent *head)
{
    struct logEvent *logIter;
    FILE *logFile = fopen(fileName, "w");

    //ensures log file was opened correctly
    if (!logFile)
    {
        return LOG_OPEN_ERROR;
    }

    logIter = head;

    //iterating over log
    while (logIter->next)
    {

        //if current log event has a valid entry
        if (logIter->entry)
        {
            /*remove last newline character. too many newlines are printed otherwise.
            this happens because "%s\n" must be the argument to fprintf because providing
            just "%s" results in a warning. everything prints fine, but it's better to not
            have a warning and get the same result.*/
            substr(logIter->entry, 0, strLen(logIter->entry) - 1, logIter->entry);
            fprintf(logFile, "%s\n", logIter->entry);
        }

        logIter = logIter->next;
    }

    //prints the last item in the log to the log file, if it exists
    if (logIter->entry)
    {
        fprintf(logFile, "%s\n", logIter->entry);
    }

    fclose(logFile);

    return 0;
}

/*
    iterates over log and frees all memory associated with it
*/
void freeLog(struct logEvent *head)
{
    struct logEvent *tmp;

    //iterates linked list
    while (head->next)
    {
        tmp = head;
        head = head->next;

        //frees allocated memory for string and then frees the node in the linked list
        free(tmp->entry);
        free(tmp);
    }

    //frees the last node in the log linked list as well as associated memory
    free(head->entry);
    free(head);
}

/*
    prints all of the info contained in the log
*/
void printLog(struct logEvent *logList)
{
    struct logEvent *entry = logList;

    //iterates over log and pritns entrys in log list
    while (entry->next)
    {
        printf("%s\n", entry->entry);
    }

    //prints last entry in log
    printf("%s\n", entry->entry);
}

/*
    handles all logic dealing with logging to monitor and logging to file
*/
void logIt(char *line, struct logEvent *logList, bool logToMon, bool logToFile)
{
    if (logToMon)
    {
        printf("%s", line);
    }

    if (logToFile)
    {
        appendToLog(logList, line);
    }
}

