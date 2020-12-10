// header files
#include <stdio.h>
#include "MetaDataAccess.h"

/*
 Function Name: main
 Algorithm: driver function to test metadata uplaod
 Precondition: none
 Postcondition: returns zero on suzzess
 Exceptions: none
 Nontes: demonstrates development and use of metadata file uplad
 */
int main( int argc, char **argv )
{
    int mdAccessResult;
    char mdFileName [ MAX_STR_LEN ];
    OpCodeType *mdData;
    
    printf( "\nMeta Data File Upload Program\n" );
    printf( "============================\n" );
    
    //checks command line arguments
    if( argc < 2 )
    {
        printf( "\nERROR: Program requires file name for meta data files" );
        printf( " as command line argument \n" );
        printf( "Program terminated. \n" );
        
        return 1;  // return non-normal progr4am exit status
    }
    
    copyString( mdFileName, argv[ 1 ] );
    mdAccessResult = getMetaDataFile( mdFileName, &mdData );
    
    //check for successful upload
    if( mdAccessResult == NO_ERR )
    {
        displayMetaData( mdData );
    }
    
    //otherwise unsuccessful uplaod
    else
    {
        displayMetaDataError( mdAccessResult );
    }
    
    mdData = clearMetaDataList( mdData );
    
    printf( "\n" );
    
    //return success
    return 0;
}

