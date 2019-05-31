#include <iostream>
#include <string>

#include <CImg.h>

#include "element.hpp"
#include "function.hpp"
#include "image.hpp"

// TODO: Compile faster

int main()
{
    Image img(700, 700, 1, 3);
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
            f.plot(&img, fColour, 3, std::make_pair(-3.14159*3, 3.14159*3), std::make_pair(-5, 5))->display(display);
       }
    }

    std::cout << "Bye bye!";
    return EXIT_SUCCESS;
}