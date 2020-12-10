#include <stdio.h>

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
         double sum;

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
         sum = augend + addend;

	// display result to the user
               // function: pintf
         printf( "\nThe sum of %d and %d is %4.2f\n\n", addend, augend, sum );

	// shut down
            //return success;
	return 0;
	}

