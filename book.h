// book.h
// Dan Ivy
// spring 2017
// definition of BOOK class
#ifndef _BOOK_H_
#define _BOOK_H_

#include "word.h"

#include <string>

#include "types.h"




using namespace std;



class Book
{
public:
    // creates an empty book
    Book() = default;
    //Book( const Book& source );
    //Book& operator=( const Book& source );

	// creates a book from the data inside the directory dir
	Book( const char* dir );

    // loads a book from dir, including metadata, freq.1.txt,
    // and zdata, if available. returns false on error
    bool load( const string& dir, bookID id );

    // saves the probabilities that this book belongs to
    // each topic to [dir]/topics. returns false on error
    bool save( const string& dir ) const;

	// metadata
 	const string& getTitle() const;
	const string& getAuthor() const;
	const size_t getId() const;

	// returns the probability that this book is from topic
	float getTopicProb( size_t topic ) const;
    // add val to the topic probability
	void addTopicProb( size_t topic, float val );
    // sets the topic probabilities to 0
    void setTopicProbToZero();

	// normalizes the topic probabilities the book/words
	void normalize();
    void normalizeWords();

	// returns a reference to the i'th word in the book
    Word& getWord( wordID i );

    // returns the number of unique words in the book
    size_t numWords() const;
	

private:
	// the id of the book
	bookID xId;

	// title and author of book
	string xTitle;
	string xAuthor;

	// the length of the book
	wordCount xLength;

	// each word that appears in the book
	vector<Word> xWords;
	
	// the probability that the book is from each topic
	float xPTopic[ NUM_TOPICS ];
};


#endif
