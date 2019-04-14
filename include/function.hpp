#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <iostream>
#include <map>
#include <deque>

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

    struct point {
        public:
            float x = 0;
            float y = 0;
    };

    auto gradientStepFunction(const std::pair<point, point> latestPoints, float xstep, float ystep, int l_value) const noexcept {
        auto pointsToDraw = std::make_shared<std::vector<point>>();
        auto getDistance = [](point a, point b) -> float { 
            point a_b;
            if(a.x > b.x){
                a_b.x = a.x-b.x;
            }
            else{
                a_b.x = b.x-a.x;
            }
            if(a.y > b.y){
                a_b.y = a.y-b.y;
            }
            else{
                a_b.y = b.y-a.y;
            }
            /*std::cout << "[getDistance]: b(" << b.x << ',' << b.y << "), a_b(" << a_b.x << ',' << a_b.y << ")=" << pow(
                pow(a_b.x, 2)+pow(a_b.y,2), 
                   0.5f);*/
            return pow(
                pow(a_b.x, 2)+pow(a_b.y,2), 
                   0.5f);
        };
        auto isWithinDistance = [getDistance](float distance, point a, point b) -> bool {
            return distance >= getDistance(a, b);
        };
        for (int i = 0; i <= 2 * l_value; i++)
        {
            point p;
            point nulla;
            nulla.x = 0;
            nulla.y = 0;
            p.x = i - l_value;
            for (int j = 0; j <= 2 * l_value; j++)
            {
                p.y = j - l_value;
                if(isWithinDistance(l_value, nulla, p)){
                    pointsToDraw->push_back({latestPoints.first.x + p.x*xstep, latestPoints.first.y + p.y*ystep});
                }
            }
        }
        return pointsToDraw;
    };


    auto plot(Image* img, unsigned char colour[3], unsigned int thickness, std::pair<int, int> xrange, std::pair<int, int> yrange) const noexcept {
        // Variable map - what values we have defined as accessible as keys and values to VariableElements
        std::map<char, float> varMap = {
            { 'x', 0.0f },
            { 'p', 3.14159f }
        };

        std::pair<point, point> latestPoints;                                                       // The last two points we plotted.

        float xstep = ((float)(xrange.second - xrange.first)/(float)img->xSize);                    // The difference between the numbers two pixels on the x-axis represent.
        float yScaleFactor = img->ySize / (yrange.second - yrange.first); 
        float xScaleFactor = img->xSize / (xrange.second - xrange.first); 

        for(unsigned int i = 0; i < img->xSize; i++){
            varMap['x'] = (float)xrange.first + (float)i*xstep;                                     // Set the current value of x - what number are we at currently?
            point alpha = {varMap['x'], call(varMap)};                                              // The next point to plot.

            latestPoints.second = latestPoints.first;                                               // Update the latestPoints to account for the newly calculated point.
            latestPoints.first = alpha;

            if(varMap['x'] == xrange.first){    // HACK: Can't exactly use our gradient step algorithm on the first point
                img->image->draw_point(alpha.x, alpha.y*yScaleFactor, colour, 1);
                continue;
            }

            auto pointsToDraw = gradientStepFunction(latestPoints, xstep, 1/yScaleFactor, 5);
            for(auto point : *pointsToDraw){
                img->image->draw_point(img->xSize/2 + point.x*xScaleFactor, img->ySize/2 - point.y*yScaleFactor, colour, 1);
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
