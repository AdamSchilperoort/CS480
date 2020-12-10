#include <stdlib.h>
#include <stdio.h>
#include "strUtils.h"
#include "booleans.h"
#include "pcb.h"
#include "errors.h"

//ALL FILE-RELATED UTILITIES

/*
    reads in config file and stores all data in configValues struct
        * opens file fileName
        * reads lines from config file:
            * parses line and sets data to configValues struct
        * returns 0 on success, error code otherwise
*/
int readConfigFile(char *fileName, struct configValues *settings)
{
    char *line = calloc(100, sizeof(char));
    int pos, len, num;
    float ver;
    FILE *config = fopen(fileName, "r");

    //ensure openin config file did not fail
    if (!config)
    {
        return CONFIG_FILE_OPEN_ERROR;
    }

    //iterating over file, storing file line in line variable
    while (fgets(line, 100, config))
    {
        //allocating value and locating ':' in line
        char *value = (char *)calloc(100, sizeof(char));
        pos = substrPos(line, ":");

        //if successfully found substr logic
        if (pos >= 0)
        {
            len = strLen(line);

            //store everything after ':' in line into value
            substr(line, pos, len, value);

            //removes all non-alphanumeric & non-symbol characters (i.e. newlines) from value
            removeNonSymbols(value);

            //version/phase logic
            if (strContains(line, "Version/Phase"))
            {

                //converting value to float
                ver = s2f(value);

                //error checking version
                if (ver > 10.0f || ver < 0.0f)
                {
                    return VERSION_PHASE_VALUE_ERROR;
                }

                //putting version value in struct
                settings->ver = ver;
            }

            //log file logic
            else if (strContains(line, "Log File"))
            {

                //allocating memory for logging type
                settings->logPath = (char *)calloc(strLen(value) + 1, sizeof(char));

                //logic for setting logPath in struct
                if (strCmp(settings->logTo, "File") || strCmp(settings->logTo, "Both"))
                {
                    strCopy(value, settings->logPath);
                }
                else
                {
                    strCopy("N/A", settings->logPath);
                }
            }

            //CPU scheduling logic
            else if (strContains(line, "CPU Scheduling Code"))
            {

                //allocates cpu shceduling type memory
                settings->cpuSched = (char *)calloc(strLen(value) + 1, sizeof(char));

                //logic for setting cpu scheduling type in struct if NONE is chosen type
                if (strCmp(value, "NONE"))
                {
                    strCopy("FCFS-N", settings->cpuSched);
                }

                //error if unsupported option is chosen
                else if (!checkCpuSched(value))
                {
                    return UNSUPPORTED_CPU_SCHED_ERROR;
                }

                //sets all other valid cpu shceduling types in struct
                else
                {
                    strCopy(value, settings->cpuSched);
                }
            }

            //quantum time logic
            else if (strContains(line, "Quantum Time"))
            {

                //converts value into int
                num = s2i(value);

                //error checking logic
                if (num > 100 || num < 0)
                {
                    return QUANTUM_TIME_VALUE_ERROR;
                }

                //sets struct quantum time
                settings->quantumTime = num;
            }

            //memory available logic
            else if (strContains(line, "Memory Available"))
            {

                //if memory is not listed in KB, error is returned
                if (!strContains(line, "(KB)"))
                {
                    return MDF_MEMORY_UNIT_ERROR;
                }

                //convert value to int
                num = s2i(value);

                //error checking logic
                if (num < 0 || num > 102400)
                {
                    return MEMORY_AVAIL_VALUE_ERROR;
                }

                //settings struct memoryAvailable
                settings->memoryAvailable = num;
            }

            //processor cycle time logic
            else if (strContains(line, "Processor Cycle Time"))
            {

                //converts value to int
                num = s2i(value);

                //error checking logic
                if (num < 1 || num > 1000)
                {
                    return CPU_CYCLE_TIME_VALUE_ERROR;
                }

                //sets cpu cycle time in struct
                settings->cpuCycleTime = num;
            }

            //I/O cycle time logic
            else if (strContains(line, "I/O Cycle Time"))
            {

                //converts value to int
                num = s2i(value);

                //error checking logic
                if (num < 1 || num > 10000)
                {
                    return IO_CYCLE_TIME_VALUE_ERROR;
                }

                //sets I/O cycle time in struct
                settings->ioCycleTime = num;
            }

            //log to logic
            else if (strContains(line, "Log To"))
            {

                //checks whether log to type is valid, returns error if it is not
                if (!checkLogTo(value))
                {
                    return INVALID_LOG_TO_ERROR;
                }

                //allocating memory for log to and setting it in the struct
                settings->logTo = (char *)calloc(strLen(value) + 1, sizeof(char));
                strCopy(value, settings->logTo);
            }

            //log file path logic
            else if (strContains(line, "File Path:"))
            {

                //if file path is not in current directory, return error
                if (strContains(value, "/"))
                {
                    return MDF_LOCATION_ERROR;
                }

                //allocating memory for and setting mdf file path in struct
                settings->mdfPath = (char *)calloc(strLen(value) + 1, sizeof(char));
                strCopy(value, settings->mdfPath);
            }
        }

        //frees value at every iteration; easier than figuring out clearing a string
        free(value);
    }

    free(line);
    fclose(config);

    return 0;
}

/*
    * reads in meta data file and stores the information contained in it:
        * reads line by line and stores data in line variable
        * creates simAction structs that are singly linked together
        * returns 0 on success, error code otherwise
*/
int readMetaDataFile(char *fileName, struct simAction *firstAction)
{
    bool done = FALSE;
    char *line = (char *)calloc(100, sizeof(char));
    int lineLength, pos, cmdPos, setDataResult;
    cmdPos = 0;
    char command[40];
    struct simAction *current = firstAction;

    FILE *mdf = fopen(fileName, "r");

    //ensures opening meta data file did not fail
    if (!mdf)
    {
        return MDF_OPEN_ERROR;
    }

    //iterating over file
    while (fgets(line, 100, mdf) && !done)
    {

        //ignores first and last lines of header/footer data
        if (!strContains(line, "Start Program Meta-Data") && !strContains(line, "End Program Meta-Data"))
        {
            lineLength = strLen(line);

            //iterates over the line and builds a command
            for (pos = 0; pos < lineLength; pos++)
            {
                command[cmdPos] = line[pos];
                cmdPos++;

                //if the current character is ';', stop building command and set action data
                if (line[pos] == ';')
                {

                    //allocates memory for new simAction struct
                    struct simAction *next = (struct simAction *)calloc(1, sizeof(struct simAction));

                    //null terminator for string
                    command[cmdPos] = '\0';

                    //reset command position
                    cmdPos = 0;

                    //sets data for simAction
                    setDataResult = setActionData(command, current);

                    //error checking logic for setting action data
                    if (setDataResult > 0)
                    {
                        return setDataResult;
                    }

                    //current now becomes next node in linked list
                    current->next = next;
                    current = next;
                }
            }
        }
        else if (strContains(line, "End Program Meta-Data"))
        {
            done = TRUE;
        }
    }

    fclose(mdf);
    free(line);

    return 0;
}

