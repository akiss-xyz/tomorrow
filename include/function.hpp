#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <functional>

#include "element.hpp"
#include "image.hpp"

enum logLevels{
    DEBUG   = 1,
    WARN    = 2,
    TRACE   = 4,
    SCAN    = 8,
    CALC    = 16,
};

// TODO: we could constexpr a lot of this if we get rid of the std::strings - the constop could easily be a cstring and we could still do what we need
// Furthermore, when we symbolise the function, there'll be no need for handling strings. Constexpr all the things.

// Currently 4 nElemTypes
template <unsigned int len, unsigned int nElemTypes>
class Function {
public:
    // TODO: Create a logging system that actually makes sense.
    unsigned int _debugLevel = logLevels::SCAN;

    std::vector<std::pair<char, std::function<float(float, float)>>> Keys = {
        std::make_pair('(', [](float left, float right){
                // TODO: Handle a bracket instance:
                return left + right;
            }),
        std::make_pair('%', [](float left, float right){
                return fmod(left, right);
            }),
        std::make_pair('^', [](float left, float right){
                return pow(left,  right);
            }),
        std::make_pair('*', [](float left, float right){
                return left * right;
            }),
        std::make_pair('/', [](float left, float right){
                return left / right;
            }),
        std::make_pair('+', [](float left, float right){
                return left + right;
            }),
        std::make_pair('-', [](float left, float right){
                return left - right;
            }),
    };
private:
    const Element Elems[nELems] = { 

    const char* const source[len] = ",";
    const Element func[len];
public:
 /***     Main public functions    ***/
    constexpr Function(const char* const operation) noexcept;

    constexpr void plot(Image* img) const noexcept;

    constexpr void plot(Image* img, unsigned char* colour) const noexcept;

    /*Calls the current function operation, using the current key
        set. To change the operation, call setOperation(). It's currently not possible to change the keyset on-the-fly.
    * @param [in] float x : The value of the variable in the function operation in this call.
    * @return void
    * @see Keys, setOperation(), getOperation()
    */
    constexpr float call(float x) const noexcept;

    /* Read a single float value from the operation, to the right of the current position.
    * @param [in] unsigned int pos : The current position of the scan in the string. Should be on the operation that triggered the call.
    * @param [out] int* length : A pointer to an integer that will hold the length of the offset. Used for the deletion of the term we're dealing with.
    * @return float : The value parsed from the operation string.
    * @see call(), ReadNumBack()
    */
    constexpr float ReadNum(unsigned int pos, int* length, float x) const noexcept;

    // TODO: Instead of using int* length and then taking that as an offset, make it work off of the length of the std::string
    // TODO: Test and rework the positioning. e.g. pos += 1 is a hack, make that work better.
    
    /* Read a single float value from the operation, to the left of the current position.
    * @param [in] unsigned int pos : The current position of the scan in the string. Should be on the operation that triggered the call.
    * @param [out] int* length : A pointer to an integer that will hold the length of the offset. Used for the deletion of the term we're dealing with.
    * @return float : The value parsed from the operation string.
    * @see call(), ReadNum()
    */
    constexpr float ReadNumBack(int pos, int* length, float x) const noexcept;

    /* A quick algorithm to reverse a std::string
    * @param [in] std::string& str : A reference to the string to reverse.
    * @see ReadNumBack(), ReadNum()
    */
    constexpr void reverseStr(const char* str) const noexcept;
};