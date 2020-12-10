// header files
#include "MetaDataAccess.h"

/*
 Function Name: getOpCodes
 Algorithm: opens file, acquires op code data,
            returns pointer to head of linked list
 Precondition: for correct operation, file is available, is formatted correctly,
             and has all correctly formed op codes
 Postcondition: returns pointer to head of op code linked list (for correct op)
 Exceptions: correctly and appropriately (without program failure)
           responds to and reports file access failure,
            incorrectly formatted lead or end descriptors,
            incorreclty formatted prompt, incorrect op code letter,
            incorrect op code name, op code value out of range
            incomplete file conditions
 Nontes: none
 */
int getOpCodes( char *fileName, OpCodeType **opCodeDataHead )
{
    const char READ_ONLY_FLAG[]  = "r";
    int startCount = 0, endCount = 0;
    
    OpCodeType *localHeadPtr = NULL;
    
    int accessResult;
    char dataBuffer[ MAX_STR_LEN ];
    OpCodeType *newNodePtr;
    FILE *fileAccessPtr;
    
    *opCodeDataHead = NULL;
    
    fileAccessPtr = fopen( fileName, READ_ONLY_FLAG );
    
    if( fileAccessPtr = NULL )
    {
        return MD_FILE_ACCESS_ERR;
    }
    
    if( getLineTo( fileAccessPtr, MAX_STR_LEN, COLON,
                      dataBuffer, IGNORE_LEADING_WS ) != NO_ERR
       || compareString(dataBufer, "Start Program Meta-Data Code" ) != STR_EQ )
    {
        fclose( fileAccessPtr );
        return MD_CORRUPT_DESCRIPTOR_ER;
    }
    
    newNodePtr = ( OpCodeType * ) malloc( sizeof( OpCodeType ) );
    accessResult = getOpCommand( fileAccessPtr, newNodePtr );
    
    startCount = updateStartCount( startCount, newNodePtr -> opName );
    endCount = updateEndCount( endCount, newNodePtr->opName );
    
    if( accessResult != COMPLETE_OPCMD_FOUND_MSG )
    {
        fclose( fileAccessPtr );
        *opCodeDataHead = clearMetaDataList( localHeadPtr );
        free( newNodePtr );
        return accessResult;
    }
    
    while( accessResult == COMPLETE_OPCMD_FOUND_MSG )
    {
        localHeadPtr = addNode( localHeadPtr, newNodePtr );
        accessResult = getOpCommand( fileAccessPtr, newNodePtr );
        startCount = updateStartCount( startCount, newNodePtr->opName );
        endCount = updateEndCount( endCount, newNodePtr-> opName );
    }
    
    //after loop completion, check for last op command found
    if( accessResult == LAST_OPCMD_FOUND_MSG )
    {
        if( startCount == endCount )
        {
            localHeadPtr = addNode( localHeadPtr, newNodePtr );
            accessResult = NO_ERR;
            
            if( getLineTo( fileAccessPtr, MAX_STR_LEN, PERIOD,
                          dataBuffer, ignoreLeadingWhiteSpace ) != NO_ERR
               || compareString(dataBuffer, "End Program Meta-Data Code" )
                                                != STR_EQ )
            {
                accessResult = MD_CORRUPT_DESCRIPTOR_ER;
            }
        }
        
        else
        {
            accessResult = UNBALANCED_START_END_ERR;
        }
    }
    
    //check for any errors found
    if( accessResult != NO_ERR )
    {
        //clear the op command list
        localHeadPtr = clearMetaDataList( localHeadPtr );
    }
    
    fclose( fileAccessPtr );
    free( newNodePtr );
    *opCodeDataHead = localHeadPtr;
    
    return accessResult;
}

/*
 Function Name: getOpCommand
 Algorithm: acquires one op command, verifies all parts
 Precondition: file is open, cursor is at beginning of op code
 Postcondition: find, tests, and returns op command as param
                returns status as integer, either complete op found
                or last op command found
 Exceptions: responds to and reports file access failure
            incorrectly formatted op command letter,
            incorrectly formatted op command name,
            incorrect or out of range op command val
 Nontes: none
 */
int getOpCommand( FILE *filePtr, OpCodeType *inData )
{
    const int MAX_OP_NAME_LENGTH = 10;
    const int MAX_OP_VALUE_LENGTH = 9;
    int intBuffer;
    int sourceIndex = 0, destIndex = 0;
    int accessResult;
    char strBuffer[STD_STR_LEN];
    
    accessResult = getLineTo( filePtr, STD_STR_LEN, SEMICOLON,
                             strBuffer, IGNORE_LEADING_WS );
    
    if( accessResult == NO_ERR )
    {
        inData->opLtr = strBuffer[ sourceIndex ];
    }
    
    // otherwise assume unsuccessful access
    else
    {
        inData = NULL;
        return OPCMD_ACCESS_ERR;
    }
    
    //verify op command letter
    switch( inData->opLtr )
    {
        case 'S':
        case 'A':
        case 'P':
        case 'M':
        case 'I':
        case 'O':
            break;
            
            //otherwise assume not correct case;
        default:
            inData = NULL;
            return CORRUPT_OPCMD_LETTER_ERR;
    }
    
    //loop until left paren found
    while( sourceIndex < STD_STR_LEN && strBuffer[ sourceIndex ] != LEFT_PAREN )
    {
        sourceIndex++;
    }
    
    //skip left paren element, increment source index
    sourceIndex++;
    
    //set op command text, loop until right paren found
    while( sourceIndex < STD_STR_LEN )
        && destIndex < MAX_OP_NAME_LENGTH
        && strBuffer[ sourceIndex ] != RIGHT_PAREN )
    {
        //acquire letter
        inData -> opName[ destIndex ] = strBuffer[ sourceIndex ];
        destIndex++; sourceIndex++;
        
        //set end/null character to current end of string
        inData->opName[ destIndex ] = NULL_CHAR;
    }
    
    if( checkOpString( inData-> opName ) == False )
    {
        //set struct to null
        inData = NULL;
        return CORRUPT_OPCMD_NAME_ERR;
    }
    
    sourceIndex++;
    destIndex = 0;
    
    while( sourceIndex < STD_STR_LEN
          && destIndex < MAX_OP_VALUE_LENGTH
          && isDigit( strBuffer[ sourceIndex ] ) == True )
    {
        intBuffer *= 10;
        intBuffer += (int)( strBuffer[ sourceIndex ] - '0' );
        destIndex++; sourceIndex++;
    }
    
    // check for loop overrun failure, check specified lengths
    if( sourceIndex == STD_STR_LEN || destIndex == MAX_OP_VALUE_LENGTH )
    {
        //set struct to null
        inData = NULL;
        
        return CORRUPT_OPCMD_VALUE_ERR;
    }
    
    inData->opValue = intBuffer;
    
    if( inData -> opLtr == 'S'
       && compareString( indata->opName, "end" ) == STR_EQ )
    {
        return LAST_OPCMD_FOUND_MSG;
    }
    
    return COMPLETE_OPCMD_FOUND_MSG;
}


void displayMetaData( OpCodeType *localPtr )
{
    
}


void displayMetaDataError( int code )
{
    
}


OpCodeType *clearMetaDataList( OpCodeType *localPtr )
{
    
}

