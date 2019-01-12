#include <iostream>

#include <CImg.h>

#include "function.hpp"
#include "image.hpp"

// TODO: Adding still doesn't work
// TODO: Compile faster wtf
//         Precompile the cimg library tbh

int main()
{
    Function f("5*x,");         unsigned char fColour[3] = { 255, 0, 0 };
    Function g("3*x,");         unsigned char gColour[3] = { 0, 255, 0 };
    Function h("0.3*x,");       unsigned char hColour[3] = { 0, 0, 255 };

    Image img(90*5, 120*5, 1, 3);
    unsigned char randomColour[3] = { 255, 255, 255 };
    img.drawAxes((const unsigned char*)&randomColour);

    cimg::CImgDisplay display(*img.image, "A plot.");
    img.drawAxes(randomColour);

    f.plot(&img, fColour); g.plot(&img, gColour); h.plot(&img, hColour);

    img.image->display(display);
    while(!display.is_closed()){
        display.wait();

        img.image->display(display);
    }

    return EXIT_SUCCESS;
}