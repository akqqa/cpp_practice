#define cimg_use_jpeg
#define cimg_use_png
#include "CImg.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
using namespace cimg_library;

// Algorithm:
// read characters, create a mapping of 256 range to characters evenly based on how many there are#
// calculate size of image, and split into average brightness of each size so one section = one character
// loop through and assign characters to each

std::string getCharacterSet(std::string filename) {
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()   ));
    return content;
}

std::map<int, std::string> mapCharacterDensity(std::string characterSet) {
    std::map<int, std::string> mapping = {{1, "one"}, {2, "two"}};
    return mapping;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect number of arguments\n";
        return 1;
    }

    std::string characterSet = getCharacterSet(argv[1]);
    std::cout << "characters: " << characterSet << "\n";

    std::map<int, std::string> mapping = mapCharacterDensity(characterSet);
    std::cout << "first map: " << mapping[1] << "\n";

    // CImg<unsigned char> image("marathon.jpg");
    // CImgDisplay main_disp(image, "this is the image");
    // while(true) {
        
    // }
}