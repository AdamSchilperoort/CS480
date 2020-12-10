#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES

//DATA STRUCTURES AND THEIR RELATED FUNCTIONS

//TODO: FREE_SETTINGS FUNCTION

//HOLDS CONFIG FILE INFORMATION
struct configValues
{
    float ver;

    char *mdfPath;
    char *cpuSched;
    char *logPath;
    char *logTo;

    int quantumTime;
    int memoryAvailable;
    int cpuCycleTime;
    int ioCycleTime;
};

//HOLDS ALL INFORMATION PERTAINING TO ONE ACTION
struct simAction
{
    char commandLetter;
    long assocVal;
    char *operationString;
    struct simAction *next;
};

//stores all information associated with a process
struct PCB
{
    int processNum;
    int timeRemaining;
    int memoryUsed;
    char *state;
    struct simAction *pc;
};

//creates a list of pcbs
void createPCBList(struct PCB **pcbList, struct simAction *head, struct configValues *settings);

//FREES ALL MEMORY ASSOCIATED WITH simActionS
void freeActions(struct simAction *head);

//PRINTS ALL ALL RELEVANT simAction INFORMATION FOR ALL simActions
void printSimActions(struct simAction *head);

//PRINTS ALL INFORMATION FORM A configValues STRUCT
void printConfigValues(struct configValues *src, char *fileName);

//TURNS A COMMAND STRING INTO AN ACTION
int setActionData(char *command, struct simAction *action);

//RETURNS THE NUMBER OF APPLICATIONS TO BE RUN IN A SIM ACTION LIST
int countApplications(struct simAction *head);

/*verifies that:
    * system opens at start and ends at end and both only occur once
    * applications start and end once:
        * application must close before opening another one
        * application must open before closing
    * returns ERROR_CODE or 0 if successful*/
int verifySimActions(struct simAction *head);

void setStatesReady(struct PCB **pcbList, int numProcesses);

void freePCBs(struct PCB **pcbList, int numApps);

void freeConfigValues(struct configValues *settings);

int verifySettings(struct configValues *settings);

#endif

