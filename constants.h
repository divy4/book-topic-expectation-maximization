#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <string>

#include "types.h"

const int NUM_TOPICS = 50;
const bookID NUM_BOOKS = 10000;
const wordCount NUM_TERMS = 4101;
const std::string FREQ_SET = "10000";

const std::string dataDir = "books/";

const size_t MAX_ITERATIONS = 500;
const int NUM_THREADS = 4;
const float lambda = 0.5;

#endif
