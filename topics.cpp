#include "topics.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>

#include "utils.h"

using namespace std;





void Topics::normalizeAll()
{
    for ( size_t i = 0 ; i < 1 + NUM_TOPICS ; ++i )
    {
        this->normalize( i );
    }
}





void Topics::normalize( const size_t& topic )
{
    float total = 0;
    float* data = xProb[ topic ];

    // get total
    for ( size_t i = 0 ; i < NUM_TERMS ; ++i )
    {
        total += data[ i ];
    }
    
    // normalize data
    if ( total != 0 )
    {
        for ( size_t i = 0 ; i < NUM_TERMS ; ++i )
        {
            data[ i ] /= total;
        }
    }
}





void Topics::randomize()
{
    // for each topic
    for ( size_t topic = 0 ; topic < 1 + NUM_TOPICS ; ++topic )
    {
        // for each term
        float* data = xProb[ topic ];
        for ( size_t term = 0 ; term < NUM_TERMS ; ++term )
        {
            data[ term ] = randFloat( 1, 10 );
        }
        // normalize
        this->normalize( topic );
    }
}





bool Topics::loadData( const string& filename )
{
    // open file
    ifstream file( filename, ios::binary );
    if ( !file.is_open() )
    {
        print( "Unable to open topic file\n" );
        return false;
    }

    // read data
    file.read( (char*)xProb, ( 1 + NUM_TOPICS ) * NUM_TERMS * sizeof( float ) );

    file.close();
    return true;
}





bool Topics::saveData( const string& filename ) const
{
    // open file
    ofstream file( filename, ios::binary );
    if ( !file.is_open() )
    {
        print( "Unable to open topic file\n" );
        return false;
    }
    
    // write probability data
    file.write( (char*)xProb, ( 1 + NUM_TOPICS ) * NUM_TERMS * sizeof( float ) );
    
    file.close();
    return true;
}





float Topics::getProb( const size_t& topic, const size_t& term ) const
{
    return xProb[ topic ][ term ];
}





void Topics::setProb( const size_t& topic, const size_t& term, const float& value )
{
    xProb[ topic ][ term ] = value;
}





void Topics::addProb( const size_t& topic, const size_t& term, const float& value )
{
    xProb[ topic ][ term ] += value;
}





void Topics::setProbToZero( const size_t& topic )
{
    for ( size_t i = 0 ; i < NUM_TERMS ; ++i )
    {
        xProb[ topic ][ i ] = 0;
    }
}
