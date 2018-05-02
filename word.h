// word.h
// Dan Ivy
// spring 2017
// definition of Word class
#ifndef _WORD_H_
#define _WORD_H_

#include <vector>

#include "constants.h"
#include "utils.h"

using namespace std;




class Word
{
public:
	// normalizes the probabilities of pTopic
	void normalize();

	// theid of the word being processed
	wordID id;

	// the number of times the word occurs in a document/background
	wordCount count;

	// the probability of z = topic for this word in the document
	float z[ NUM_TOPICS ];
};



#endif
