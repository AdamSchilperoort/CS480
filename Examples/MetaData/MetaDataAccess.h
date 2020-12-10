// header files
#include <stdio.h>
#include "StringUtils.h"

typedef enum { MD_FILE_ACCESS_ERR = 3,
               MD_CORRUPT_DESCRIPTOR_ERR,
               OPCMD_ACCESS_ERR,
               CORRUPT_OPCMD_LETTER_ERR,
               CORRUPT_OPCMD_NAME_ERR,
               CORRUPT_OPCMD_VALUE_ERR,
               UNBALANCED_START_END_ERR,
               COMPLETE_OPCMD_FOUND_MSG,
               LAST_OPCMD_FOUND_MSG
             } OpCodeMessages;

typedef struct OpCodeType
{
    char opLtr;
    char opName[ 100 ];  // length of op name - 99 characters
    int opValue;
    
    struct OpCodeType *next;
}   OpCodeType;
