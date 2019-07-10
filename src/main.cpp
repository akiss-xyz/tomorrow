#include <iostream>
#include <string>

#include <CImg.h>

#include "element.hpp"
#include "function.hpp"
#include "image.hpp"

// TODO: Compile faster

int main()
{
    const int imageHeight = 700;
    const int imageWidth = 700;
    //                              Between -3pi and 3pi, so that trig functions look good.
    const auto yRange = std::make_pair(-3.14159*3, 3.14159*3);
    //                              Between -5 and 5 on the x-axis.
    const auto xRange = std::make_pair(-5, 5);

    Image img(imageWidth, imageHeight, 1, 3);
    cimg::CImgDisplay display(*img.image, "Tomorrow");

    unsigned char fColour[3] = {255, 0, 0};
    unsigned char axesColour[3] = {255, 255, 255};

    img.drawAxes(axesColour);
    img.image->display(display);

    std::cout << "Enter a function, or q to exit.\n";
    while (!display.is_closed())
    {
        std::cout << "f(x)=";

        std::string source;
        std::cin >> source;

        if (source == "q" || source == "quit" || source == "Quit")
        {
            img.image->save_bmp("graph.bmp");
            display.close();
        }
        else
        {
            Function f(source);
            f.plot(&img, fColour, 3, yRange, xRange)->display(display);
       }
    }

    std::cout << "Bye bye!";
    return EXIT_SUCCESS;
}
