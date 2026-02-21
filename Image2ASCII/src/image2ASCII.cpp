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

const float CHAR_ASPECT = 0.5f; // Characters are ~half as wide as they are tall
const int OUTPUT_WIDTH = 60;

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

// Resizes the image so that one pixel = one character. Has the bonus of performing the average operation for us
CImg<unsigned char> resizeImage(CImg<unsigned char> image, int outputWidth) {
    int imgWidth = image.width();
    int imgHeight = image.height();
    float aspectRatio = (float)imgHeight / imgWidth;

    int outputHeight = outputWidth * aspectRatio * CHAR_ASPECT;

    return image.resize(outputWidth, outputHeight);
}

map<int, string> mapCharacterDensity(vector<string> characterSet, CImg<unsigned char> image, bool scaleContrast) {
    // Get brightness range of image
    image = image.RGBtoYCbCr().channel(0);

    int maxBrightness = 0;
    int minBrightness = 256;
    if (scaleContrast) {
        cimg_forXY(image, x, y) {
            unsigned char value = image(x, y);
            if (value > maxBrightness) {
                maxBrightness = value;
            }
            if (value < minBrightness) {
                minBrightness = value;
            }
        }
    }

    cout << "max brightness: " << maxBrightness << "\n";
    cout << "min brightness: " << minBrightness << "\n";

    // Divide range by number of character levels
    int numLevels = characterSet.size();
    double intervalSize = double(maxBrightness - minBrightness + 1) / numLevels;

    // Create map - unsure what will happen - can the case of the top level only being say 1 value due to rounding? dont want this
    map<int, string> mapping;
    for (int level = 0; level < numLevels; ++level) {
        int upper = int(minBrightness + intervalSize * (level  + 1)) - 1;
        // Clamp last range to maxBrightness
        if (level == numLevels - 1) {
            upper = maxBrightness;
        }

        mapping.insert({upper, characterSet[level]});
    }

    return mapping;
}

// 1st arg: character set
// 2nd arg: image
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Incorrect number of arguments\n";
        return 1;
    }

    vector<string> characterSet = getCharacterSet(argv[1]);
    cout << "characters: \n";
    for (string chars: characterSet) {
        cout << chars << "\n";
    }

    CImg<unsigned char> image(argv[2]);
    CImg<unsigned char> resizedImage = resizeImage(image, OUTPUT_WIDTH);

    map<int, string> mapping = mapCharacterDensity(characterSet, resizedImage, true);

    for (auto it = mapping.begin(); it != mapping.end(); ++it) 
        cout << it->first << " " << it->second << endl;
}