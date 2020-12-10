// header files
#include <stdio.h>
#include "StringUtils.h"

/*
 Function name: main
 Algorithm: driver function to test getStringLength function
 Precondition: none
 Postcondition: returns zero (0) on success
 Exceptions: none
 Note: demonstrates development and use of string length function
 */
int main()
{
    // initialize function/variables
    
    // create test string
    char testStr[] = "Every Good Boy Does Fine";
    
    // declare other variables
    int strLen;
    
    // disply title
    // function: printf
    printf( "\nString Length Test Program\n" );
    printf( "===========================\n\n" );
    
    // conduct string length test
    // function: getStringLength
    strLen = getStringLength( testStr );
    
    // display result to user
    // function: printf
    printf( "The length of the string \'%s\' is %d\n\n", testStr, strLen );
    
    // shut down program
    
    // return program success
    return 0;
}
