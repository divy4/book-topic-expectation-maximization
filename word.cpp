// word.cpp
// Dan Ivy
// Spring 2017
// Implementation of Word class
#include "word.h"

#include <cstring>

#include <iostream>


using namespace std;




void Word::normalize()
{
    float total = 0;
    for ( wordID i = 0 ; i < NUM_TOPICS ; ++i )
    {
        total += this->z[ i ];
    }
    
    if ( total != 0 )
    {
        for ( wordID i = 0 ; i < NUM_TOPICS ; ++i )
        {
            this->z[ i ] /= total;
        }
    }
}
