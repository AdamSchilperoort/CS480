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
    int strLen, compareResult, subStrIndex;
    char destStr[ STD_STR_LEN ];
    
    // disply title
    // function: printf
    printf( "\nString Utilities Test Program\n" );
    printf( "===============================\n\n" );
    
    // conduct string length test
    // function: getStringLength
    strLen = getStringLength( testStr );
    
    // display result to user
    // function: printf
    printf( "The length of the string \'%s\' is %d\n\n", testStr, strLen );
    
    // test copy string function
        // function: copyString
    copyString( destStr, testStr );
    
    // display copy string test result to the user
        // function: printf
    printf( "\nThe string \'%s\' was correctly copied\n", destStr );
    
    // test concatenate string function
        // function: concatenateString
    concatenateString( destStr, " In Major Keys" );
    
    // display concatenate test result to the user
        // function: printf
    printf( "\nThe string \'%s\' was correctly concatenated\n", destStr );

    // test compare string function for greater than condition
        // function: compareString
    compareResult = compareString( "Susan", "Bill" );

    // display compareString test rsult to the user
        // function: printf
    printf( "\nThe difference between Susan and Bill is %d\n", compareResult );
    
    // test compare string function for less than condition
        // function: compareString
    compareResult = compareString( "Anya", "Meghan" );
        
    // display compareString test rsult to the user
        // function: printf
    printf( "\nThe difference between Anya and Meghan is %d\n", compareResult );
    
    // test compare string function for condition with equal name
    // but one is longer than the other
        // function: compareString
    compareResult = compareString( "Will", "William" );
        
    // display compareString test rsult to the user
        // function: printf
    printf( "\nThe difference between Will and William is %d\n", compareResult );
    
    // test get substring function (magic numbers used for indicies)
        // function: getSubString
    getSubString( destStr, destStr, 20, 32 );
    
    // display getSubString test rsult to the user
        // function: printf
    printf( "\nThe substring \'%s\' was generated \n", destStr );
    
    // test find sub string
        //function: find SubString
    subStrIndex = findSubString( destStr, "ne In Ma" );
    
    //display findSubstring test result to the user
        //function printf
     printf( "\nThe substring \'ne In Ma\' starts at index %d\n", subStrIndex );
    
    // test for substring failure - not found
        // function findSubstring
    subStrIndex = findSubString( destStr, "In Major Keys" ); //won't work
    
    //output not found
    if( subStrIndex == SUBSTRING_NOT_FOUND )
    {
        printf("\nThe substring \'In Major Keys\' not found\n" );
    }
    
    // otherwise, assume that it is found
    else
    {
        //output found
        printf( "\nThe substring \'In Major Keys\' starts at index %d\n", subStrIndex );
    }
    
    // test for setting string with any upper case letter to lower case
        // function- copyString, setStrToLowerCase
    copyString( testStr, destStr );
    setStrToLowerCase( destStr, testStr );
    
    // display lower case string test rsult to the user
        // function: printf
    printf("\nThe string \'%s\' as lower case is %s\n", testStr, destStr );
    
    // shut down program
    
    // return program success
    return 0;
}
