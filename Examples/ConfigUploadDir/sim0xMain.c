// header files
#include <stdio.h>
#include "ConfigAccess.h"

/*
 Function name: main
 Algorithm: driver function to test config file upload operation
 Precondition: none
 Postcondition: returns zero (0) on success
 Exceptions: none
 Note: demonstrates development and use of config file upload operations and function
 */
int main( int argc, char **argv )
{
    // initialize function/variables
    int configAccessResult;
    char configFileName[ MAX_STR_LEN ];
    ConfigDataType *configDataPtr;
    
    // display title
        // function printf
    printf( "\nConfig File Upload Program\n" );
    printf( "============================\n\n" );
    
    // check for incorrect number of command line arguments (two)
    if( argc < 2 )
    {
        //print missing command line arg error
            // function printf
        printf( "ERROR: Program requires file name for config file " );
        printf( "as command line argument\n" );
        printf( "Program Terminated\n" );
        
        
        // return non-normal prog result
        return 1;
    }
    
    // get data from config file
         // function copyString, getConfigData
    copyString( configFileName, argv[ 1 ] );
    configAccessResult = getConfigData( configFileName, &configDataPtr );
    
    // check for successful upload
    if( configAccessResult == NO_ERR )
    {
        // display config file
        // function displayConfigData
        displayConfigData( configDataPtr );
    }
    
    // otherwise, assume failed upload
    else
    {
    // display config upload error
        // function displayConfigError
        displayConfigError( configAccessResult );
    }
    
    //shut down program

    // clear config data
         // function clearConfigData
    clearConfigData( &configDataPtr );
    
    // add endline for vertical spacing
        //function printf
    printf( "\n" );
    
    //return program success
    return 0;
}
