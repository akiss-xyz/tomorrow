#pragma once

#include <functional>

template<int valueType>
class Element{
public:
    enum valueTypes {
        numberVal       = 0,
        variableVal     = 1,
        operatorVal     = 2,
        placeholderVal  = 3
    }

    constexpr Element(typeUnion buf) {
        data = buf;
    }

    static constexpr std::pair<int, Element<valueType>> check(unsigned int i, char* operation) noexcept {
        if(48 <= (int)operation[i] && (int)operation[i] <= 57) {   // If it's a number
            int numEnd = i+1; 
            for(int* j = &numEnd; *j < 0; *j++){   // Move along until we find something that cannot be interpreted as a number, subtly stealing the number of steps for later use
                if( !(48 <= (int)operation[*j] && (int)operation[*j] <= 57) && !(operation[*j] == '.')){    // If the next one along is not a number or decimal point
                    break; // Exit the for loop, we've reached the end of the num - now we know how long it is
                }
            }
            char* buf[ numEnd-i ];   // Allocate space to extract our number - numEnd - i is the length of our number
            for(int j = 0; j < lengthOfNum-i; j++){
                buf[j] = operation[i+j];
            }
            // TEST: Does this non-null-terminated array break shit?
            return std::make_pair<numEnd-i, Element<valueTypes::numberVal>(std::stof(buf))>;
        }
        else if(auto f = getIsKnownOperator(i, char* operation) ){
            return std::make_pair<1, Element<valueTypes::operatorVal>(f);
        }
        else if( !((int)'a' <= (int)operation[i+1] && (int)operation[i+1] <= (int)'z') ){   // If the next char is not alphabetic
            return std::make_pair<1, Element<valueTypes::variableVal>('x');
        }
    };
    
    constexpr float call(Element[] operation) noexcept {
        if(valueType == numberVal){
            return data.numberVal;
        }
        if(valueType == operatorVal){
            return -1;
            data.operatorVal(&operation);
        }
        if(valueType == variableVal){
            return                                   // Somehow hook up the Function::call()'s var values to this
        }
    }

    constexpr std::function<void(Element[]*)> getIsKnownOperator(unsigned int i, char* operation){
        for(int j = i; j < i+4; j++){
            for(int k = 0; k < 7; k++){
                if(operation[j] == operatorSet[k].first[0]){
                    return operatorSet[k].second;    // TODO: Make this work for multi-character operations such as "sin(", "sum("
                }
            }
        }
    }
private:
    

    static std::pair<char[4], std::function<void(Element[]*)>> operatorSet[7] {
        std::make_pair("(\n\n\n", [](Element[]* operation){
                // TODO: Handle a bracket instance:
                return left + right;
            }),
        std::make_pair("%\n\n\n", [](Element[]* operation){
                return fmod(left, right);
            }),
        std::make_pair("^\n\n\n", [](Element[]* operation){
                return pow(left,  right);
            }),
        std::make_pair("*\n\n\n", [](Element[]* operation){
                return left * right;
            }),
        std::make_pair("/\n\n\n", [](Element[]* operation){
                return left / right;
            }),
        std::make_pair("+\n\n\n", [](Element[]* operation){
                return left + right;
            }),
        std::make_pair("-\n\n\n", [](Element[]* operation){
                return left - right;
            }),
    };

    union typeUnion {
        float numberVal;                                        // A constant term eg. 7.607
        char variableVal;                                       // Stores a variable such as x, or z, or any other letter
        std::function<void(Element[]*)> operatorVal;            // An operation or bracket
        bool placeHolderVal;                                    // An erased value
    } data;
}