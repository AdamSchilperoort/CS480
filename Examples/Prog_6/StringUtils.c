// header files
#include "StringUtils.h"

// global constants
const int MAX_STR_LEN = 200;
const int STD_STR_LEN = 80;
const char NULL_CHAR = '\0';
const int SUBSTRING_NOT_FOUND = -101;
const char SPACE = ' ';

/*
 Function name: getStringLength
 Algorithm: find the length of the string, up to the null character
 Precondition: given C-style with null character at end
 Postcondition: return the number of characters (int) from the beginning to the null character
 Exceptions: none
 Note: limit test loop to maximize characters for safety
 */
int getStringLength( char *testStr )
{
    // initialize function/variables
    int index = 0;
    
    // loop up to null character or limit
    while( index < MAX_STR_LEN && testStr[ index ] != NULL_CHAR )
    {
        //increment the counter index
        index++;
    }
    
    // end loop
    
    // return the counter index value
    
    return index;
}

/*
 Function name: copyString
 Algorithm: copies contents of one string into another
 Precondition: given C-style source string, having a null character ('\0')
               at end of string; destination string is passed in
               as a parameter with enough memory to accept the source string
 Postcondition: destination string contains an exact copy of source string
 Exceptions: none
 Note: limit test loop to maximize characters for safety
 */
void copyString( char *destination, char *source )
{
    // initialize function/variables
    int index = 0;
    //loop until null character is found in source string
       //loop limited to MAX_STR_LEN
    while( index < MAX_STR_LEN && source[ index ] != NULL_CHAR )
    {
        // assign source character to destination element
        destination[ index ] = source[ index ];
        
        // increment index
        index++;
        
        //assign null character to next destination element
        destination[ index ] = NULL_CHAR;
    }

}

/*
 Function name: concatenateString
 Algorithm: concatenates or appends contents of one string onto the
            end of another
 Precondition: given C-style source string, having a null character ('\0')
            at end of string; destination string is passed in
            as a parameter with enough memory to accept the source string
 Postcondition: destination string contains its original string with
             the source string appended or concatenated to the end of it
 Exceptions: none
 Note: limit test loop to maximize characters for safety
 */
void concatenateString( char *destination, char *source )
{
    int destIndex = getStringLength( destination );
    int sourceIndex = 0;
    
    while( sourceIndex < MAX_STR_LEN && source[ sourceIndex ] != NULL_CHAR )
    {
        destination[ destIndex ] = source[ sourceIndex ];
        
        sourceIndex++; destIndex++;
        
        destination[ destIndex ] = NULL_CHAR;
    }
}

/*
 Function name: compareString
 Algorithm: compares two strings alphabetically such that:
            if oneStr < otherStr, the function returns a value < 0
            if oneStr > otherStr, the function returns a value > 0
            if oneStr == other Str, the function returns a 0
            if two strings are identical up to the point that one is longer,
            the difference in lengths will be returned
 Precondition: given C-style source string, having a null character ('\0')
                at end of string
 Postcondition: integer value returned as specified
 Exceptions: none
 Note: limit test loop to maximize characters for safety
 */
int compareString( char *oneStr, char *otherStr )
{
    int index = 0;
    int difference;
    
    //loop to end of one of the two strings, limited to MAX
    while( index < MAX_STR_LEN && oneStr[ index ] != NULL_CHAR
                             && otherStr[ index ] != NULL_CHAR )
    {
        difference = oneStr[ index ] - otherStr[ index ];
        
        if( difference != 0 )
        {
            return difference;
        }
        
        index++;
    }
    // assume strings are equal at this point, return string length difference
    return getStringLength( oneStr ) - getStringLength( otherStr );
}

/*
 Function name: getSubString
 Algorithm: captures sub string within larger string between two
            inclusive indicies
 Precondition: given C-style source string, having a null character ('\0')
            at end of string, and another string parameter with enough
            memory to hold the resulting substring
 Postcondition: substring is returned as a parameter
 Exceptions: empty string returned if any of the index parameters
            are out of range
 Note: copy of source string is made internally to protect from aliasing
 */
void getSubString( char *destStr, char *sourceStr,
                                 int startIndex, int endIndex )
{
    int sourceStrLen = getStringLength( sourceStr );
    int destIndex = 0;
    int sourceIndex = startIndex;
    char *tempSourceStr;
    
    if( startIndex >= 0 && startIndex <= endIndex && endIndex < sourceStrLen )
    {
        tempSourceStr = (char *) malloc( sourceStrLen +1 );
        copyString( tempSourceStr, sourceStr );
        
        // loop across requested substring
        while( sourceIndex <= endIndex )
        {
            destStr[ destIndex ] = tempSourceStr[ sourceIndex ];
            destIndex++; sourceIndex++;
            destStr[ destIndex ] = NULL_CHAR;
        }
        
        free( tempSourceStr );
    }
    else
    {
        destStr[ 0 ] = NULL_CHAR;
    }
}


/*
 Function name: findSubString
 Algorithm: linear search for given substring within a given test string
 Precondition: given a C-style test string, having a null character ('\0')
                at end of string, and given search string with
                a null character ('\n') at the end of that string
 Postcondition: index of substring location returned,
                or SUBSTRING_NOT_FOUND constant is returned
 Exceptions: none
 Note: none
*/
int findSubString( char *testStr, char *searchSubStr )
{
    int testStrLen = getStringLength( testStr );
    int masterIndex = 0;
    int searchIndex, internalIndex;
    
    while( masterIndex < testStrLen )
    {
        internalIndex = masterIndex;
        searchIndex = 0;
        
        while( internalIndex <= testStrLen
                && testStr[ internalIndex ] == searchSubStr[ searchIndex ] )
        {
            internalIndex++; searchIndex++;
            
            if( searchSubStr[ searchIndex ] == NULL_CHAR )
            {
                return masterIndex;
            }
        }
        
        masterIndex++;
    }
    // assume tests have failed at this point
    return SUBSTRING_NOT_FOUND;
}


/*
 Function name: setStrToLowerCase
 Algorithm: iterates through string, set all upper case letters
            to lower case without changing any other characters
 Precondition: given a C-style test string, having a null character ('\0')
            at end of string, and destination string parameter
            is passed with enough memory to hold the lower case string
 Postcondition: all upper case letters in given string are set
            to lower case; no change to any other characters
 Exceptions: limit on string loop in case incorrect string format
 Note: copy of source string is made internally to protect from aliasing
 */
void setStrToLowerCase( char *destStr, char *sourceStr )
{
    int strLen = getStringLength( sourceStr );
    char *tempStr = (char *) malloc( strLen + 1 );
    int index = 0;
    
    copyString( tempStr, sourceStr );
    
    while( index < MAX_STR_LEN && tempStr[ index ] != NULL_CHAR )
    {
        destStr[ index ] = setCharToLowerCase( tempStr[ index ] );
        index++;
        destStr[ index ] = NULL_CHAR;
    }
    
    free( tempStr );
}


/*
 Function name: setCharToLowerCase
 Algorithm: tests character param for upper case, changes it to lower case,
             makes no changes if not upper case
 Precondition: given character value
 Postcondition: upper case letter is set to lower case, otherwise
                character is returned unchanged
 Exceptions: none
 Note: none
 */
char setCharToLowerCase( char testChar )
{
    if( testChar >= 'A' && testChar <= 'Z' )
    {
        testChar = (char) ( testChar - 'A' + 'a' );
    }
    
    return testChar;
}


/*
 Function name: getLineTo
 Algorithm: finds given text in file, skipping white space if specified,
            stops searching at given character or length
 Precondition: file is open with valid file pointer;
               char buffer has adequate memory for data;
               stop character and length are valid
 Postcondition: ignores leading white space if specified;
               captures all printable characters and stores in string buffer;
 Exceptions: returns INCOMPLETE_FILE_ERR if no valid data found;
             returns NO_ERR if successful operation
 Note: none
 */
int getLineTo( FILE *filePtr, int bufferSize, char stopChar,
                  char *buffer, Boolean omitLeadingWhiteSpace )
{
    int charIndex = 0;
    int statusReturn = NO_ERR;
    Boolean bufferSizeAvailable = True;
    int charAsInt;
    
    charAsInt = fgetc( filePtr );
    
    while( omitLeadingWhiteSpace == True
            && charAsInt != (int) stopChar
            && charIndex < bufferSize
            && charAsInt <= (int) SPACE )
    {
        charAsInt = fgetc( filePtr );
    }
    
    while( charAsInt != (int) stopChar && bufferSizeAvailable == True )
    {
        if( isEndOfFile( filePtr ) == True )
        {
            return INCOMPLETE_FILE_ERR;
        }
        
        if( charAsInt >= (int) SPACE )
        {
            buffer[ charIndex ] = (char) charAsInt;
            charIndex++;
        }
        
        buffer[ charIndex ] = NULL_CHAR;
        
        if( charIndex < bufferSize - 1 )
        {
            charAsInt = fgetc( filePtr );
        }
        
        else
        {
            bufferSizeAvailable = False;
            statusReturn = INPUT_BUFFER_OVERRUN_ERR;
        }
    }
    
    return statusReturn;
}


/*
 Function name: isEndOfFile
 Algorithm: reports end of file, using feof
 Precondition: file is open with valid file pointer
 Postcondition: reports end of file
 Exceptions: none
 Note: none
 */
Boolean isEndOfFile( FILE *filePtr )
{
    if( feof( filePtr ) != 0 )
    {
        return True;
    }
    
    return False;
}
