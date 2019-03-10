#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <iostream>
#include <map>

#include "element.hpp"
#include "image.hpp"

class Function {
public:
    /* Debug and help methods */
    void printOpState(const Element::operationState* const opState, bool all) const noexcept;
    std::string toString() const noexcept;

    struct operatorInfo {
    public:
        bool isBinary = false;
        bool isUnary = false;
    };

    /* Data related to this function */
    static const unsigned int nOfOperators = 6;
    static const std::map<const char, Function::operatorInfo> operatorSet;
    // List of our element data
    std::vector<std::shared_ptr<Element>> _data;

    /* Main functionality */
    Function(const std::string& source) noexcept;
    float call(std::map<char, float> varMap) const noexcept;
    cimg::CImg<unsigned char>* plot(Image* img, unsigned char colour[3], int xthickness, int ythickness, std::pair<int, int> xrange, std::pair<int, int> yrange) const noexcept {
        std::map<char, float> varMap = {
            { 'x', 0.0f },
        };
        std::pair<float, float> latest;
        for(int i = 0; i < (int)img->xSize; i++){
            float x = (float)xrange.first + (float)i*(((float)xrange.second - (float)xrange.first)/((float)img->xSize));
            varMap['x'] = x;

            auto toScale = img->ySize / (yrange.second - yrange.first);
            

            // This is not a good algorithm for thickness. It should be a lot more like a guaranteed cross-sectional length, and we calculate vectors between points.
            for(int jx = 0; jx < (xthickness-1)/2; jx++){
                for(int jy = 0; jy < (ythickness-1)/2; jy++){
                    img->image->draw_point(i + jx, img->ySize/2 - (int)std::round(call(varMap)*toScale) + jy, colour, 1);
                    img->image->draw_point(i - jx, img->ySize/2 - (int)std::round(call(varMap)*toScale) - jy, colour, 1);
                }
            }
        }
        return img->image;
    };

    struct elemContainer {
    public:
        bool successfulParse;
        std::shared_ptr<Element> elem;
        unsigned int elemLength = 1;
 
        struct parsedElem {
        public:
            bool isOperatorType = false;
            bool isValueType = false;
        } latestElem;

        elemContainer(bool isSuccessful, std::shared_ptr<Element> element, unsigned int elemLen, parsedElem parse)
        : successfulParse(isSuccessful), elem(element), elemLength(elemLen), latestElem(parse) {};
    };

private:
    /* Parsing utility functions */
    elemContainer getElementAt(unsigned int pos, const std::string& source, elemContainer::parsedElem latest) const noexcept;

    static inline elemContainer getOperator(unsigned int pos, const std::string& source, elemContainer::parsedElem latest) noexcept;

    static inline bool isNumeric(const std::string& source, unsigned int start, elemContainer::parsedElem latest) noexcept;

    static inline bool isVariable(unsigned int start, const std::string& source, elemContainer::parsedElem latest) noexcept;
};
