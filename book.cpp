// book.cpp
// Dan Ivy
// Spring 2017
// Implementation of Book class
#include "book.h"

#include <cstdlib>
#include <fstream>

#include "background.h"
#include "utils.h"
#include "constants.h"

using namespace std;





bool Book::load( const string& dir, bookID id )
{
    xId = id;

    // open frequency file
    string fullDir = dir + "/" + getBookFolder( id ) + "/";
    ifstream freqFile( fullDir + "count." + FREQ_SET + ".id" );
    if ( !freqFile.is_open() )
    {
        print( "Unable to open frequency file\n" );
        return false;
    }
    
    string term;
    wordID termId = 0;
    wordCount count;
    ssize_t xWordsSize = 0;

    while ( !freqFile.eof() )
    {
        // read frequency and zdata for current word
        freqFile >> termId;
        freqFile >> term;
        freqFile >> count;
       
        // push word to words
        ++xWordsSize;
        xWords.resize( xWordsSize );
        xWords[ xWordsSize - 1 ].id = termId;
        xWords[ xWordsSize - 1 ].count = count;
      
        // update book/background metadata
        xLength += count;
    }
    freqFile.close();
    
    // set up document ->topic probabilities
    ifstream zFile( fullDir + "zdata" );
    if ( zFile.is_open() )
    {
        // read from file
        zFile.read( (char*)xPTopic, NUM_TOPICS * sizeof( float ) );
        zFile.close();
    }
    else
    {
        // randomly generate numbers
        for ( size_t i = 0 ; i < NUM_TOPICS ; ++i )
        {
            xPTopic[ i ] = randFloat( 1, 100 );
        }
    }
    normalize();

    return true;
}





bool Book::save( const string& dir ) const
{
    // open file
    ofstream zFile( dir + getBookFolder( xId ) + "/topics" );
    if ( !zFile.is_open() )
    {
        print( "Unable to open topics file\n" );
        return false;
    }

    // write to file
    for ( size_t i = 0 ; i < NUM_TOPICS ; ++i )
    {
        zFile << xPTopic[ i ] << endl;
    }

    // close file
    zFile.close();
}





const string& Book::getTitle() const
{
    return xTitle;
}





const string& Book::getAuthor() const
{
    return xAuthor;
}





const size_t Book::getId() const
{
    return xId;
}





float Book::getTopicProb( size_t topic ) const
{
    return xPTopic[ topic ];
}





void Book::addTopicProb( size_t topic, float val )
{
    xPTopic[ topic ] += val;
}





void Book::setTopicProbToZero()
{
    for ( size_t i = 0 ; i < NUM_TOPICS ; ++i )
    {
        xPTopic[ i ] = 0;
    }
}





void Book::normalize()
{
    float sum = 0;

    // sum values
    for ( size_t i = 0 ; i < NUM_TOPICS ; ++i )
    {
        sum += xPTopic[ i ];
    }

    // normalize
    if ( sum != 0 )
    {
        for ( size_t i = 0 ; i < NUM_TOPICS ; ++i )
        {
            xPTopic[ i ] /= sum;
        }
    }
}





void Book::normalizeWords()
{
    // normalize each book
    size_t size = xWords.size();
    for ( size_t i = 0 ; i < size ; ++i )
    {
        xWords[ i ].normalize();
    }
}





Word& Book::getWord( wordID i )
{
    return xWords[ i ];
}





size_t Book::numWords() const
{
    return xWords.size();
}
