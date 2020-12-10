// header files
#include <stdio.h>

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
