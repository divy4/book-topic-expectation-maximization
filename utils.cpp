#include "utils.h"

#include <cmath>
#include <cstdlib>
#include <ctime>

const int bookFolderWidth = 5;



std::string getBookFolder(bookID id) {
    return std::string(bookFolderWidth - floor(log10(id)) - 1, '0') + std::to_string(id) + '/';
}



void openAndCheck(std::ifstream& stream, const std::string& filename) {
    stream.open(filename);
    if (!stream.is_open()) {
        std::cerr << "Unable to open " << filename << std::endl;
    }
}



void openAndCheck(std::ofstream& stream, const std::string& filename) {
    stream.open(filename);
    if (!stream.is_open()) {
        std::cerr << "Unable to open " << filename << std::endl;
    }
}



float randFloat( float a, float b )
{
    return a + ( ( b - a ) / RAND_MAX ) * rand();
}

