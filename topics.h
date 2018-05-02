#ifndef _TOPIC_H_
#define _TOPIC_H_

#include <string>
#include <vector>
#include <utility>

#include "constants.h"

using namespace std;



class Topics
{
public:
    // normalize data
    void normalizeAll();
    void normalize( const size_t& topic );

    // randomizes probability data and normalizes the data afterwards
    void randomize();

    // load probability data. returns false on error
    bool loadData( const string& filename );

    // saves file
    bool saveData( const string& filename ) const;

    // get/set functions
    float getProb( const size_t& topic, const size_t& term ) const;
    void setProb( const size_t& topic, const size_t& term, const float& value );
    void addProb( const size_t& topic, const size_t& term, const float& value );

    // set all probability values of topic to 0
    void setProbToZero( const size_t& topic );
    

private:
    // an array of topics (xProb[ NUM_TOPICS ] topic is the background)
    float xProb[ 1 + NUM_TOPICS ][ NUM_TERMS ];
};


#endif
