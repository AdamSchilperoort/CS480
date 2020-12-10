#include "pcb.h"
#include "booleans.h"
#include "errors.h"
#include "strUtils.h"
#include <stdio.h>
#include <stdlib.h>

//DATA STRUCTURE FUNCTIONS

/*
    frees all actions in a linked list of actions
        * iterates over list and frees the previous nodes
          and their associated values
*/
void freeActions(struct simAction *head)
{
    struct simAction *tmp;

    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->operationString);
        free(tmp);
    }
}

/*
    prints all sim actions and their relevant info
        * iterates over the list and prints all of the info associates
          with the actions in the list
*/
void printSimActions(struct simAction *head)
{
    struct simAction *ptr = head;

    while (ptr->next)
    {
        printf("Op code letter: %c\n", ptr->commandLetter);
        printf("Op code name  : %s\n", ptr->operationString);
        printf("Op code value : %ld\n\n", ptr->assocVal);
        ptr = ptr->next;
    }
}

/*
    prints all the info contained in a configValue struct
*/
void printConfigValues(struct configValues *src, char *fileName)
{

    printf("Version                : %f\n", src->ver);
    printf("Program file name      : %s\n", src->mdfPath);
    printf("CPU schedule selection : %s\n", src->cpuSched);
    printf("Quantum time           : %d\n", src->quantumTime);
    printf("Memory Available       : %d\n", src->memoryAvailable);
    printf("Process cycle rate     : %d\n", src->cpuCycleTime);
    printf("I/O cycle rate         : %d\n", src->ioCycleTime);
    printf("Log to selection       : %s\n", src->logTo);
    printf("Log file name          : %s\n\n", src->logPath);
}

/*
    sets data for a simAction struct
        * iterates over command
            * grabs command letter  
            * builds an operationString
            * grabs and converts associated value
        * sets data to sim action struct
        * returns 0 on success, error code otherwise
*/
int setActionData(char *command, struct simAction *action)
{
    long value;
    int cmdLength, pos, opFlag, opIter;
    char currentChar, opStr[40];

    cmdLength = strLen(command);
    pos = 0;
    value = 0;
    opIter = 0;
    opFlag = FALSE; //operation string flag; alerts function that the operation string is being parsed now
    currentChar = command[pos];

    //if (); don't exist in the string, return error
    if (!strContains(command, "(") || !strContains(command, ")") || !strContains(command, ";"))
    {
        return CORRUPTED_MDF_ERROR;
    }

    while (pos < cmdLength)
    {

        //skip whitespace and check for an uppercase character
        if (!(currentChar == ' ') && charIsUpper(currentChar))
        {
            action->commandLetter = command[pos];
        }

        //check for '(' and set the operationString flag to true
        else if (currentChar == '(')
        {
            opFlag = TRUE;
        }

        //starts the operation string finished parsing logic
        else if (currentChar == ')')
        {

            //add null terminator to operation string
            opStr[opIter] = '\0';

            //returns error if command letters S or A are not used with "start" or "end"
            if ((!strCmp(opStr, "start") && !strCmp(opStr, "end") && (action->commandLetter == 'S' || action->commandLetter == 'A')))
            {
                return SA_OP_STRING_ERROR;
            }

            //returns error if command letter P is not used with operation string "run"
            else if (!strCmp(opStr, "run") && action->commandLetter == 'P')
            {
                return P_OP_STRING_ERROR;
            }

            //returns error if command letter I is not used with operation string "hard drive" or "keyboard"
            else if (!strCmp(opStr, "hard drive") && !strCmp(opStr, "keyboard") && action->commandLetter == 'I')
            {
                return I_OP_STRING_ERROR;
            }

            //returns error if command letter O is not used with operation string "printer" or "monitor"
            else if (!strCmp(opStr, "hard drive") && !strCmp(opStr, "printer") && !strCmp(opStr, "monitor") && action->commandLetter == 'O')
            {
                return O_OP_STRING_ERROR;
            }

            //returns error if command letter M is not used with operation string "allocate" or "access"
            else if (!strCmp(opStr, "allocate") && !strCmp(opStr, "access") && action->commandLetter == 'M')
            {
                return M_OP_STRING_ERROR;
            }

            //allocates operationString memory as leng of opStr + 1 for null terminator
            action->operationString = (char *)calloc(strLen(opStr) + 1, sizeof(char));
            strCopy(opStr, action->operationString);
            opFlag = FALSE;
        }

        //logic for building operation string
        else if (opFlag)
        {
            opStr[opIter] = currentChar;
            opIter++;
        }

        //logic for dealing with associated value
        else if (!opFlag && !(currentChar == ';') && charIsNum(currentChar))
        {
            value *= 10;               //moves current numbers in value one position to the left
            value += c2i(currentChar); //adds new number to value
        }

        //moves to the next character in the mdf command string
        pos++;
        currentChar = command[pos];
    }

    //returns error if value is negative
    if (value < 0)
    {
        return NEGATIVE_MDF_VALUE_ERROR;
    }

    //sets associated value and returns with success code
    action->assocVal = value;
    return 0;
}

/*
    counts the number of A(start)0; commands in the meta data
        * iterates over simAction linked list
        * counts the number of times A(start)0; appears
        * returns the number of A(start)0; appearances
*/
int countApplications(struct simAction *head)
{
    int count = 0;
    struct simAction *tmp = head;

    while (tmp->next)
    {
        if (tmp->commandLetter == 'A' && strCmp(tmp->operationString, "start"))
        {
            count++;
        }

        tmp = tmp->next;
    }

    return count;
}

/*
    verifications:
        * system:
            * system starts first
            * system starts once
            * system end is the final call
        * application:
            * application must start before ending
            * application must end before starting again
        * returns ERROR_CODE or 0 if successful
*/
int verifySimActions(struct simAction *head)
{
    //boolean flags for showing what is currently active
    bool osStart = FALSE;
    bool appStart = FALSE;
    bool osEnd = FALSE;
    bool first = TRUE;

    struct simAction *tmp = head; //linked list iterator
    char *opString;
    char cmd;

    while (tmp->next)
    {
        cmd = tmp->commandLetter;
        opString = tmp->operationString;

        //logic for dealing with the first command in the linked list
        if (first)
        {
            first = FALSE;

            //first command must be start OS, returns error otherwise
            if (cmd == 'S' && strCmp(opString, "start"))
            {
                osStart = TRUE;
            }
            else
            {
                return OS_START_ERROR;
            }
        }

        //logic for dealing with everything after the OS starts
        else if (osStart)
        {

            //OS may only start once, returns error otherwise
            if (cmd == 'S' && strCmp(opString, "start"))
            {
                return MULTIPLE_OS_START_ERROR;
            }

            //if the OS has not ended logic
            else if (!osEnd)
            {

                //logic for dealing with applications
                if (cmd == 'A')
                {
                    //if an application has already started logic
                    if (appStart)
                    {
                        //starting app while another is open results in error
                        if (strCmp(opString, "start"))
                        {
                            return CONCURRENT_APP_START_ERROR;
                        }

                        //otherwise "end" operation string must have been issued
                        else
                        {
                            appStart = FALSE;
                        }
                    }

                    //logic for dealing with if an application is not currently open
                    else
                    {
                        if (strCmp(opString, "start"))
                        {
                            appStart = TRUE;
                        }

                        //if app hasn't started and end is called, result is error
                        else
                        {
                            return APP_END_TIME_ERROR;
                        }
                    }
                }

                //logic for dealing with operating system end call
                else if (cmd == 'S' && strCmp(opString, "end"))
                {
                    osEnd = TRUE;
                }
            }

            //anything coming after OS end call results in error
            else
            {
                return OS_END_ERROR;
            }
        }
        //not sure if this is redundant or not, but if OS doesn't start first this will hit maybe?
        else if (!osStart)
        {
            return OS_START_ERROR;
        }

        //iterates linked list
        tmp = tmp->next;
    }

    return 0;
}

/*
    constructs a list of PCB structs:
        * creates a PCB for each app in linked list:
            * increments process numbers
            * selects A(start); item as program counter from simAction linked list
            * calculates time remaining for process
            * sets state to "new"
*/
void createPCBList(struct PCB **pcbList, struct simAction *head, struct configValues *settings)
{
    struct simAction *actionIter;
    char *new = "new";
    int processNum = 0;
    struct PCB *controlBlock;
    bool isCalculating = FALSE; //flag to tell function that calculation of time is taking place for time remaining
    int timeRemaining = 0;

    actionIter = head;

    while (actionIter->next)
    {
        //if application start is found logic
        if (actionIter->commandLetter == 'A' && strCmp(actionIter->operationString, "start"))
        {
            controlBlock = (struct PCB *)calloc(1, sizeof(struct PCB));
            //sets process number, state to new, and program counter
            controlBlock->processNum = processNum;
            controlBlock->state = new;
            controlBlock->memoryUsed = 0;
            controlBlock->pc = actionIter;

            //turns on calculating flag
            isCalculating = TRUE;
        }

        //logic for calculating time remaining for a PCB
        if (isCalculating)
        {

            //command letter P calculation logic
            if (actionIter->commandLetter == 'P')
            {
                //increase time remaining by associated value * cpu cycle time
                timeRemaining += actionIter->assocVal * settings->cpuCycleTime;
            }

            //command letters I & O calculation logic
            else if (actionIter->commandLetter == 'I' || actionIter->commandLetter == 'O')
            {
                //increase time remaining by associated value * I/O cycle time
                timeRemaining += actionIter->assocVal * settings->ioCycleTime;
            }

            //if application end reached logic
            else if (actionIter->commandLetter == 'A' && strCmp(actionIter->operationString, "end"))
            {
                //set time remaining, put PCB in PCBList, turn off calculating flagg, & increment process number
                controlBlock->timeRemaining = timeRemaining;
                timeRemaining = 0;
                pcbList[processNum] = controlBlock;
                isCalculating = FALSE;
                processNum++;
            }
        }

        //iterate over the linked list
        actionIter = actionIter->next;
    }
}

/*
    iterates over pcbList and sets state of all PCBs to "ready"
*/
void setStatesReady(struct PCB **pcbList, int numProcesses)
{
    int processNum;
    struct PCB *controlBlock;

    for (processNum = 0; processNum < numProcesses; processNum++)
    {
        controlBlock = pcbList[processNum];
        controlBlock->state = "ready";
    }
}

/*
    frees PCBList memory:
        * frees all PCBs in the array
        * frees the PCB array at the end
*/
void freePCBs(struct PCB **pcbList, int numApps)
{
    int pos;

    for (pos = 0; pos < numApps; pos++)
    {
        free(pcbList[pos]);
    }
}

/*
    frees all of the memory for configValues struct:
        * frees all allocated strings from struct
        * frees the rest of the struct at the end
*/
void freeConfigValues(struct configValues *settings)
{
    free(settings->mdfPath);
    free(settings->cpuSched);
    free(settings->logPath);
    free(settings->logTo);
    free(settings);
}

//verifies system settings
int verifySettings(struct configValues *settings)
{
    if (!settings->ver)
    {
        return VERSION_NOT_PARSED;
    }
    else if (!settings->mdfPath)
    {
        return MDF_PATH_NOT_PARSED;
    }
    else if (!settings->cpuSched)
    {
        return CPU_SCHED_NOT_PARSED;
    }
    else if (!settings->logPath)
    {
        return LOG_PATH_NOT_PARSED;
    }
    else if (!settings->logTo)
    {
        return LOG_TO_NOT_PARSED;
    }
    else if (!settings->quantumTime)
    {
        return QUANTUM_NOT_PARSED;
    }
    else if (!settings->memoryAvailable)
    {
        return MEM_AVAIL_NOT_PARSED;
    }
    else if (!settings->cpuCycleTime)
    {
        return CPU_CYCLE_NOT_PARSED;
    }
    else if (!settings->ioCycleTime)
    {
        return IO_CYCLE_NOT_PARSED;
    }

    return 0;
}

