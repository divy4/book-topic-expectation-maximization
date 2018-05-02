#include "em.h"

#include <cmath>
#include <ctime>
#include <fstream>
#include <queue>
#include <unistd.h>

#include "background.h"
#include "book.h"
#include "topics.h"
#include "utils.h"




Topics* topics = NULL;
Book* books = NULL;
Background* background = NULL;





void emInit()
{
    time_t startTime = time( NULL );
    print( "Loading EM data\n" );


    // allocate topics
    print( "Loading topics\n" );
    topics = new Topics;
    if ( topics == NULL )
    {
        print( "Unable to allocate topics\n" );
        exit( 1 );
    }
    topics->randomize();
    print( "%zu topics loaded\n", NUM_TOPICS );


    // background
    print( "Loading background data\n" );
    background = new Background( dataDir + "count." + FREQ_SET + ".id" );
    if ( background == NULL )
    {
        print( "Unable to allocate background\n" );
        exit( 1 );
    }
    print( "Background data loaded\n" );


    // books
    print( "Loading books" );
    books = new Book[ NUM_BOOKS ];
    if ( books == NULL )
    {
        print( "\nUnable to allocate books\n" );
    }
    for ( bookID id = 1 ; id <= NUM_BOOKS ; ++id )
    {
        books[ id - 1 ].load( dataDir, id );
        if ( id % 1000 == 0 )
        {
            print( "." );
        }
    }
    print( "\n%zu books loaded\n", NUM_BOOKS );

    
    print( "Loaded data in %d seconds\n", time(NULL) - startTime );
}





void emClose()
{
    print( "Saving data\n" );
    for ( bookID i = 0 ; i < NUM_BOOKS ; ++i )
    {
        books[ i ].save( dataDir );
    }
    topics->saveData( dataDir + "topics" );
    print( "Done\n" );

    // free data
    print( "Freeing EM data\n" );
    delete topics;
    topics = NULL;
    delete[] books;
    books = NULL;
    delete background;
    background = NULL;
    print( "Done\n" );
}





void emMainEStep()
{
    for ( wordID i = 0 ; i < NUM_TERMS ; ++i )
    {
        // p(z=B) = lambda * p(w|B)
        background->setZ( i, lambda * topics->getProb( NUM_TOPICS, i ) );
    }
    background->normalize();
}





void emMainMStep()
{
    for ( wordID i = 0 ; i < NUM_TERMS ; ++i )
    {
        // p(w|B) = c(w,B) * p(z=B)
        topics->setProb( NUM_TOPICS, i, background->getCount( i ) *
                                        background->getZ( i ) );
    }
    topics->normalize( NUM_TOPICS );
}





void emWorkerEStep( const size_t& id )
{
    bookID start = id * NUM_BOOKS / NUM_THREADS;
    bookID end = ( id + 1 ) * NUM_BOOKS / NUM_THREADS;

    // for each book
    for ( bookID bookNum = start ; bookNum < end ; ++bookNum )
    {
        Book& book = books[ bookNum ];
       
        // for each term in the book
        wordID size = book.numWords();
        for ( wordID termNum = 0 ; termNum < size ; ++termNum )
        {
            Word& word = book.getWord( termNum );
            // for each topic
            for ( size_t topic = 0 ; topic < NUM_TOPICS ; ++topic )
            {               
                // p(z=topic) = pi( book, topic ) * p( word | topic )
                word.z[ topic ] = book.getTopicProb( topic ) *
                    topics->getProb( topic, book.getWord( termNum ).id );
            }

            // normalize z probability of word
            word.normalize();
        }
    }
}





void emWorkerMStep( const size_t& id )
{
    // set pi values
    bookID bookStart = id * NUM_BOOKS / NUM_THREADS;
    bookID bookEnd = ( id + 1 ) * NUM_BOOKS / NUM_THREADS;
    // for each book
    for ( bookID bookNum = bookStart ; bookNum < bookEnd ; ++bookNum )
    {
        Book& book = books[ bookNum ];

        // set topic probability to 0
        book.setTopicProbToZero();
        
        // for each word in the book
        wordID size = book.numWords();
        for ( wordID wordNum = 0 ; wordNum < size ; ++wordNum )
        {
            Word& word = book.getWord( wordNum );

            // for each topic
            for ( size_t topic = 0 ; topic < NUM_TOPICS ; ++topic )
            {
                // pi += c( word, book ) * ( 1 - p(z=B) ) * p(z=topic)
                book.addTopicProb( topic, word.count *
                                        ( 1 - background->getZ( word.id ) ) *
                                        word.z[ topic ] );
            }
        }

        // normalize pi values
        book.normalize();
    }

    // set topic values
    size_t topicStart = id * NUM_TOPICS / NUM_THREADS;
    size_t topicEnd = ( id + 1 ) * NUM_TOPICS / NUM_THREADS;
    // for each topic
    for ( size_t topic = topicStart ; topic < topicEnd ; ++topic )
    {
        topics->setProbToZero( topic );

        // for each book
        for ( bookID bookNum = 0 ; bookNum < NUM_BOOKS ; ++bookNum )
        {
            Book& book = books[ bookNum ];
            
            // for each word in book
            wordID size = book.numWords();
            for ( wordID wordNum = 0 ; wordNum < size ; ++wordNum )
            {
                Word& word = book.getWord( wordNum );

                // p(w|topic) += c(word,book) * ( 1 - p(z=B) ) * p(z=topic)
                topics->addProb( topic,
                                word.id,
                                word.count *
                                ( 1 - background->getZ( word.id ) ) *
                                word.z[ topic ] );
            }
        }

        // normalize topic
        topics->normalize( topic );
    }
}

