#include <cstdlib>
#include <iostream>
#include <queue>

#include "background.h"
#include "constants.h"
#include "topics.h"




int main( int argc, char** argv )
{
    if ( argc != 4 )
    {
        cout << "Usage: bin/readTopics <id file> <topic file> <terms/topic>" << endl;
        exit( 1 );
    }

    Topics* topics = new Topics;
    topics->loadData( argv[2] );

    Background* background = new Background( argv[1] );

    int numToPrint = atoi( argv[ 3 ] );
   
    for ( size_t topic = 0 ; topic < NUM_TOPICS ; ++topic )
    {
        priority_queue< pair<float, string> > q;
        
        // push terms
        for ( size_t term = 0 ; term < NUM_TERMS ; ++term )
        {
            q.push( make_pair( topics->getProb( topic, term ), background->getTerm( term ) ) );
        }

        cout << "===== topic " << topic << " =====" << endl;
        // pop top terms
        for ( int i = 0 ; i < numToPrint ; ++i )
        {
            cout << q.top().second << ' ' << q.top().first << endl;
            q.pop();
        }
    }


    return 0;
}
