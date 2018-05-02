#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "book.h"

#include <string>

#include "types.h"

using namespace std;



class Background
{
public:
    Background( const string& filename );

    // normalize z data
    void normalize();

    // accessors
    string getTerm( wordID term ) const;
    
    wordCount getCount( wordID term ) const;

    float getZ( wordID term ) const;
    void setZ( wordID term, const float& value );

private:
    string xTerms[ NUM_TERMS ];
    wordCount xCount[ NUM_TERMS ];
    float xZ[ NUM_TERMS ];
};



#endif
