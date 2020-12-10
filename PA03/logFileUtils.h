#include <sys/time.h>
#include "pcb.h"
#include "booleans.h"

#ifndef LOG_FILE_UTILS
#define LOG_FILE_UTILS

//stores all operations in a linked list
struct logEvent
{
    char *entry;
    struct logEvent *next;
};

void createLogHeader(struct logEvent *logList);

void appendToLog(struct logEvent *logList, char *entry);

void appendSettingsToLog(struct logEvent *logList, struct configValues *settings);

void appendSimActionsToLog(struct logEvent *logList, struct simAction *head);

int createLogFile(char *fileName, struct logEvent *head);

void freeLog(struct logEvent *head);

void printLog(struct logEvent *logList);

void logIt(char *line, struct logEvent *logList, bool logToMon, bool logToFile);

#endif

