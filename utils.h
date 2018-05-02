#pragma once

#include <fstream>
#include <iostream>
#include <cstdio>
#include <vector>

#include "types.h"


enum idMode_t {
    ID,
    noID,
    genID
};

// prints out data to stderr
#define print(...) fprintf( stderr, __VA_ARGS__ )

// sleeps for a while
#define pause(...) usleep( 1000000 )

std::string getBookFolder(bookID id);

void openAndCheck(std::ifstream& stream, const std::string& filename);
void openAndCheck(std::ofstream& stream, const std::string& filename);

// generates a random float in [a,b]
float randFloat( float a, float b );
