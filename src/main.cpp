#include <iostream>
#include <string>

#include <CImg.h>

#include "element.hpp"
#include "function.hpp"
#include "image.hpp"

// TODO: Adding still doesn't work
// TODO: Compile faster wtf

int main()
{   
    Function add("2.75+7.33");
    std::cout << add.toString() << '\n';
    std::cout << add.call(std::map<char,float>({ {'x', 9.3} })) << '\n';

    Function take("2.75-7.33");
    std::cout << take.toString() << '\n';
    std::cout << take.call(std::map<char,float>({ {'x', 9.3} })) << '\n';

    Function mult("2.75*7.33");
    std::cout << mult.toString() << '\n';
    std::cout << mult.call(std::map<char,float>({ {'x', 9.3} })) << '\n';

    Function div("2.75/7.33");
    std::cout << div.toString() << '\n';
    std::cout << div.call(std::map<char,float>({ {'x', 9.3} })) << '\n';
    
    Function exp("2.75^7.33");
    std::cout << exp.toString() << '\n';
    std::cout << exp.call(std::map<char,float>({ {'x', 9.3} })) << '\n';

    Function bidmas("2.75-7.33+7.33*2-3/2");
    std::cout << bidmas.toString() << '\n';
    std::cout << bidmas.call(std::map<char,float>({ {'x', 9.3} })) << '\n';
    
    Function xVal("2*x+7");
    std::cout << xVal.toString() << '\n';
    std::cout << xVal.call(std::map<char,float>({ {'x', 4} })) << '\n';

	Function polynomial("13.254*x^3+2*x^2-13*x+15/2");
	std::cout << polynomial.toString() << '\n';
	std::cout << polynomial.call(std::map<char, float>({ {'x', -1.27197529f} })) << '\n';

	Function _3dequation("x/2+y/3");
	std::cout << _3dequation.toString() << '\n';
	std::cout << _3dequation.call(std::map<char, float>({ {'x', 2}, {'y', 3} })) << '\n';
    return EXIT_SUCCESS;
}