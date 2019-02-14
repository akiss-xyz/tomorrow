#pragma once

#include <CImg.h>
#define cimg cimg_library

class Image {
public:
    const unsigned int xSize;
    const unsigned int ySize;
    const unsigned int zSize;
    const unsigned int nColourChannels;

    cimg::CImg<unsigned char>* image;

    Image(unsigned int xSize = 60, unsigned int ySize = 60, unsigned int zSize = 60, unsigned int nColourChannels = 1);

    // unused param: unsigned int padding
    // TODO: Styles and scale, maybe check out the drawaxes func in cimg?
    void drawAxes(const unsigned char* const colour) noexcept;

private:
};