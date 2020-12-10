#include "mathUtils.h"

//ALL MATH UTILS

/*
    raises and int to a power
*/
float raiseToPower(int base, int power)
{
    int currentPower;

    //set value to 1 so if power = 0, the work is already done
    float value = 1;

    //if power is positive
    if (power > 0) 
    {

        //iterates up to power and multiplies value by base each time
        for (currentPower = 0; currentPower < power; currentPower++) 
        {
            value *= base;
        }
    }

    //if poewr is negative
    else if (power < 0)
    {
        //iterates down to power and divides by base each time
        for (currentPower = 0; currentPower > power; currentPower--) 
        {
            value /= base;
        }
    }
    
    return value;
}

