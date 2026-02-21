#define cimg_use_jpeg
#define cimg_use_png
#include "CImg.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace cimg_library;
using namespace std;

// Algorithm:
// read characters, create a mapping of 256 range to characters evenly based on how many there are#
// calculate size of image, and split into average brightness of each size so one section = one character
// loop through and assign characters to each

vector<string> getCharacterSet(string filename) {
    ifstream ifs(filename);
    vector<string> characterSet;
    for (string line; getline(ifs, line);) {
        characterSet.push_back(line);
    }
    return characterSet;
}

map<int, string> mapCharacterDensity(vector<string> characterSet) {
    map<int, string> mapping = {{1, "one"}, {2, "two"}};
    return mapping;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Incorrect number of arguments\n";
        return 1;
    }

    vector<string> characterSet = getCharacterSet(argv[1]);
    cout << "characters: \n";
    for (string chars: characterSet) {
        cout << chars << "\n";
    }

    map<int, string> mapping = mapCharacterDensity(characterSet);
    cout << "first map: " << mapping[1] << "\n";

    // CImg<unsigned char> image("marathon.jpg");
    // CImgDisplay main_disp(image, "this is the image");
    // while(true) {
        
    // }
}