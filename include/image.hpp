#pragma once

#include <CImg.h>
#define cimg cimg_library

class Image {
public:
    const unsigned int xSize = 60;
    const unsigned int ySize = 45;
    const unsigned int zSize = 1;
    const unsigned int nColourChannels = 3;

    cimg::CImg<unsigned char>* image;

private:
public:
    Image(unsigned int xSize, unsigned int ySize, unsigned int zSize, unsigned int nColourChannels);

    // unused param: unsigned int padding
    // TODO: Styles and scale, maybe check out the drawaxes func in cimg?
    void drawAxes(const unsigned char* const colour) noexcept;

private:
};