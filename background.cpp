#include "background.h"

#include <fstream>

#include "utils.h"





Background::Background( const string& filename )
{
    // open file
    ifstream file( filename );
    if ( !file.is_open() )
    {
        print( "Unable to open term file\n" );
        exit( 1 );
    }

    // read terms
    wordID temp;
    for ( wordID i = 0 ; i < NUM_TERMS ; ++i )
    {
        file >> temp;
        file >> xTerms[ i ];
        file >> xCount[ i ];
    }

    file.close();
}





void Background::normalize()
{
    float total = 0;

    // get sum
    for ( wordID i = 0 ; i < NUM_TERMS ; ++i )
    {
        total += xZ[ i ];
    }

    // normalize
    if ( total != 0 )
    {
        for ( wordID i = 0 ; i < NUM_TERMS ; ++i )
        {
            xZ[ i ] /= total;
        }
    }
}





string Background::getTerm( wordID term ) const
{
    return xTerms[ term ];
}





wordCount Background::getCount( wordID term ) const
{
    return xCount[ term ];
}





float Background::getZ( wordID term ) const
{
    return xZ[ term ];
}





void Background::setZ( wordID term, const float& value )
{
    xZ[ term ] = value;
}

