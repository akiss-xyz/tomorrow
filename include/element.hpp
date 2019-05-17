#pragma once

#include <functional>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <map>

class Element {
public:      
    struct operationState {
        // A list of the operations we do at each execution level, for example, one level includes addition and subtraction. This is iterated through in order to carry out calculations following the correct order.
        static const std::vector<std::vector<std::string>> opLevels;

        // A vec of shared_ptrs to our actual elements that comprise the function.
        std::vector<std::shared_ptr<Element>> data;

        // Stores the values passed into the function call for any and all variables in the expression e.g. the current value of x
        std::map<char, float> variableValues;

        static std::map<std::string, std::function<float(float, float)>> signatureToFuncMap;
           
        // Where we are in the element vec (index)
        unsigned int scanPosition = 0;

        // This takes values from operationState::opLevels - represents which operations we're currently concerned with.
        std::vector<std::string> executionLevel;

        // A boolean to tell computationally expensive value types to only compute if an operator calls them.
        // E.g., when calling a function, every element is called. These are not value calls, so that computationally expensive elements do not run needlessly.
        // On the other hand, when an Operator Element is being called, these are value calls - these operators need to know the value of the element itself.
        bool isValueCall = false; 
    };

    virtual std::string toString() const noexcept;

    virtual float call(operationState* opState) const noexcept;

    virtual char getType() const noexcept;

    static float binaryOperation(float val, operationState* opState) noexcept;
};

// Stores numeric values, currently only as floats.
class NumericElement : public Element {
private:
    float _data;
public:
    NumericElement(float source) : _data(source) {};

    std::string toString() const noexcept override;

    char getType() const noexcept override;

    float call(operationState* opState) const noexcept override;
};

// The Element type that represents an operator (+, -, ^, sin()) in our function. Currently only does one character ops.
class OperatorElement : public Element {
private:
    std::string _data;
public:
    OperatorElement(std::string source) : _data(source) {};
    OperatorElement(char csource) { std::stringstream ss; ss << csource; _data = ss.str(); }

    std::string toString() const noexcept override;

    char getType() const noexcept override;

    // TODO: Gotta get cleaned up.
    float call(operationState* opState) const noexcept override;
};

// Represents any constants or variables in a Function. Takes values from opState->variableValues.
class VariableElement : public Element {
private:
    char _data;
	bool _sign;
public:
    VariableElement(char source, bool sign) : _data(source), _sign(sign) {};

    std::string toString() const noexcept override;

    char getType() const noexcept override;

    float call(operationState* opState) const noexcept override;
};

// Forward Declaration of Function: Promise the Bracketelement class that the Function class exists
class Function;

class BracketElement : public Element {
private:
    std::shared_ptr<Function> _data;

public:
    BracketElement(std::shared_ptr<Function> func) : _data(func) {};

    std::string toString() const noexcept override;

    char getType() const noexcept override;

    float call(operationState* opState) const noexcept override;
};