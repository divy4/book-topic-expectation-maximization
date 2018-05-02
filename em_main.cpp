#include <iostream>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "em.h"
#include "utils.h"

using namespace std;



// global data
bool running = true;
pthread_barrier_t barrier;





// thread routines
void* thread_routine( void* data );

// signal handler for SIGINT
void handleSIGINT( int i );





int main( int argc, char** argv )
{
    // initialize EM data
    emInit();

    if ( pthread_barrier_init( &barrier, NULL, NUM_THREADS + 1 ) != 0 )
    {
        print( "Unable to create barrier\n" );
        exit( 1 );
    }

    // set signal handler
    struct sigaction action;
    action.sa_handler = handleSIGINT;
    if ( sigaction( SIGINT, &action, NULL ) == -1 )
    {
        print( "Unable to set SIGINT handler\n" );
        exit( 1 );
    }

    // create threads
    pthread_t threads[ NUM_THREADS ];
    for ( size_t i = 0 ; i < NUM_THREADS ; ++i )
    {
        if ( pthread_create( threads + i, NULL, thread_routine, (void*)i ) != 0 )
        {
            print( "Unable to create thread\n" );
            exit( 1 );
        }
    }
    
    print( "Main thread ready\n" );

    pthread_barrier_wait( &barrier );

    // exit loop if interrupted
    size_t iteration = 0;
    while( running )
    {
        ++iteration;
        print( "Running iteration %zu E-step\n", iteration );

        // E step
        emMainEStep();

        // E to M step
        pthread_barrier_wait( &barrier );

        // exit loop if interrupted
        if ( !running )
        {
            break;
        }

        print( "Running iteration %zu M-step\n", iteration );

        // M step
        emMainMStep();

        // M to E step
        pthread_barrier_wait( &barrier );

    }

    print( "Main thread closed\n" );
   
    // join threads
    for ( size_t i = 0 ; i < NUM_THREADS ; ++i )
    {
        pthread_join( threads[ i ], NULL );
    }

    // free EM data
    emClose();

    return 0;
}





void* thread_routine( void* data )
{
    size_t id = (size_t)data;

    // ignore sigint
    sigset_t sigmask;
    sigemptyset( &sigmask );
    sigaddset( &sigmask, SIGINT );
    if ( pthread_sigmask( SIG_BLOCK, &sigmask, NULL ) != 0 )
    {
        print( "Unable to set signal mask\n" );
        exit( 1 );
    }

    print( "Thread %zu ready\n", id );

    pthread_barrier_wait( &barrier );

    // exit loop if interrupted
    while ( running )
    {
        // E step
        emWorkerEStep( id );

        // E to M step
        pthread_barrier_wait( &barrier );

        // exit loop if interrupted
        if ( !running )
        {
            break;
        }

        // M step
        emWorkerMStep( id );
        
        // M to E step
        pthread_barrier_wait( &barrier );
    }

    print( "Thread %zu closed\n", id );

    return NULL;
}





void handleSIGINT( int i )
{
    running = false;
}

