// header files
#include <stdio.h> //file operations
#include <stdlib.h> //dynamic memory operations

// create global constants across files
typedef enum { False, True } Boolean;
typedef enum { NO_ERR,
               INCOMPLETE_FILE_ERR,
               INPUT_BUFFER_OVERRUN_ERR } StringManipCode;

extern const int STD_STR_LEN;
extern const int SUBSTRING_NOT_FOUND;
extern const char SPACE;


// function prototypes
int getStringLength( char *testStr );
void copyString( char *destination, char *source );
void concatenateString( char *destination, char *source );
int compareString( char *oneStr, char *otherStr );
void getSubString( char *destStr, char *sourceStr,
                  int startIndex, int endIndex );
int findSubString( char *testStr, char *searchSubStr );
void setStrToLowerCase( char *destStr, char *sourceStr );
char setCharToLowerCase( char testChar );
int getLineTo( FILE *filePtr, int bufferSize, char stopChar,
              char *buffer, Boolean omitLeadingWhiteSpace );
Boolean isEndOfFile( FILE *filePtr );
