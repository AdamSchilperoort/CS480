// global constants
const int MAX_STR_LEN = 200;
const char NULL_CHAR = '\0';

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
