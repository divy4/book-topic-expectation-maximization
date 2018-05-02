#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>

#include "constants.h"
#include "utils.h"

using namespace std;


// generates a query vector from the books with ids [ids]
void getQuery( float* query, const int& numIds, char** ids );

// generates a score between a query and a book with id [id]
float getScore( float* query, const size_t& id );

// prints results of a query result of id, score
void printResult( const size_t& id, const float& score );





int main( int argc, char** argv )
{
    if ( argc < 3 )
    {
        cerr << "Usage: bin/searchTopics <num results> <query document 1> ..."
             << endl;
        exit( 1 );
    }

    // get query vector
    float query[ NUM_TOPICS ];
    getQuery( query, argc - 2, argv + 2 );
    
    // queue of results (score, id)
    priority_queue< pair<float, size_t> > results;
    for ( size_t id = 1 ; id <= NUM_BOOKS ; ++id )
    {
        // test if id was in query
        string strId = to_string( id );
        bool inQuery = false;
        for ( size_t i = 2 ; i < argc ; ++i )
        {
            if ( strId == argv[ i ] )
            {
                inQuery = true;
                break;
            }
        }

        if ( !inQuery )
        {
            results.push( make_pair( getScore( query, id ), id ) );
        }
    }

    // print results
    long numResults = atol( argv[ 1 ] );
    for ( long result = 0 ; result < numResults ; ++result )
    {
        printResult( results.top().second, results.top().first );
        results.pop();
    }
    
    return 0;
}





void getQuery( float* query, const int& numIds, char** ids )
{
    // set query to 0
    for ( size_t topic = 0 ; topic < NUM_TOPICS ; ++topic )
    {
        query[ topic ] = 0;
    }

    // for each input id
    for ( int idNum = 0 ; idNum < numIds ; ++idNum )
    {
        // open the file
        size_t id = atoi(ids[idNum]);
        ifstream file( ( dataDir + getBookFolder(id) + "/topics" ).c_str() );
        if ( !file.is_open() )
        {
            cerr << "Unable to open query file" << endl;
            exit( 1 );
        }
        
        // for each topic
        for ( size_t topic = 0 ; topic < NUM_TOPICS ; ++topic )
        {
            float value;
            file >> value;
            query[ topic ] += value;
        }

        // close file
        file.close();
    }

    // normalize query
    for ( size_t i = 0 ; i < NUM_TOPICS ; ++i )
    {
        query[ i ] /= numIds;
    }
}




float getScore( float* query, const size_t& id )
{
    ifstream file( dataDir + getBookFolder( id ) + "/topics" );
    
    float score = 0;
    
    if ( file.is_open() )
    {
        for ( size_t topic = 0 ; topic < NUM_TOPICS ; ++topic )
        {
            // read value from file
            float value;
            file >> value;
            
            // add to score
            score += query[ topic ] * value;
        }
    }
    else
    {
        cout << "Unable to open "
             << dataDir
             << getBookFolder( id )
             << "/topics"
             << endl;
    }

    return score;
}





void printResult( const size_t& id, const float& score )
{
    // get name of book
    string name = "";
    ifstream metadata( dataDir + getBookFolder( id ) + "/metadata.txt" );
    if ( metadata.is_open() )
    {
        if ( !metadata.eof() )
        {
            getline( metadata, name );
        }
        metadata.close();
    }

    // print result!
    cout << id
         << ' '
         << score
         << ' '
         << name
         << endl;
}
