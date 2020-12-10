#include <stdio.h>

/*
Function Name: addValues
Algorithm: accept two interger values, add them, returns double sum
Precondition: two values are int
Postcondition: return double sum as specificed
Exceptions: none
Note: demonstrate creation of supporting function

*/
double addValues( int oneVal, int anotherVal )
   {

    return (double)( oneVal + anotherVal );  //temp stub return 
   }

/*
Function Name: main
Algorithm: accept two interger values, add them to a doubl sum,
		then display the results
Precondition: none
Postcondition: return 0 on success
Exceptions: none
Note: demonstrate input, output, and simple math

*/
int main()
	{
	// initialize function/variables
         int augend, addend;
         double sum = 0;

	// display title
	   // function: printf
        printf( "\nAddition Program\n" );
        printf( "==================\n" );

	// get input from the user

		// get augend
                   //function: printf, scanf
                printf( "\nEnter augend: " );
                scanf( "%d", &augend );
                

		// get addend
                    // function: printf, scanf
                printf( "\nEnter addend: " );
                scanf( "%d", &addend );

	// add values
            // function: addValues
         sum = addValues( augend, addend );

	// display result to the user
               // function: pintf
         printf( "\nThe sum of %d and %d is %4.2f\n\n", addend, augend, sum );

	// shut down
            //return success;
	return 0;
	}

