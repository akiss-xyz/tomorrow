#include "image.hpp"

using namespace cimg_library;

Image::Image(unsigned int xSize, unsigned int ySize, unsigned int zSize, unsigned int nColourChannels) : xSize(xSize), ySize(ySize), zSize(zSize), nColourChannels(nColourChannels)
{
    image = new CImg<unsigned char>(xSize, ySize, zSize, nColourChannels, 0);
}

// unused param: unsigned int padding
void Image::drawAxes(const unsigned char *const colour) noexcept
{
    // Y-Axis
    image->draw_line(xSize / 2, 0, xSize / 2, ySize, colour, 1);
    // X-Axis
    image->draw_line(0, ySize / 2, xSize, ySize / 2, colour, 1);
}