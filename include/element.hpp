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

template<class Ty>  // HACK: Literally only a template because I couldn't sort out the header sort of hierarchy to have both element.hpp and function.hpp be able to use this correctly
// Note: Pass as a pointer or const ref. A data structure used in the execution of functions. Allows us to access (read and write) all the necessary information at any level through the chain of execution.
struct operationState {
    // A list of the operations we do at each execution level, for example, one level includes addition and subtraction. This is iterated through in order to carry out calculations following the correct order.
    static const std::vector<std::vector<char>> opLevels;

    // A vec of shared_ptrs to our actual elements that comprise the function.
    std::vector<std::shared_ptr<Ty>> data;

    // Stores the values passed into the function call for any and all variables in the expression e.g. the current value of x
    std::map<char, float> variableValues;

    // Where we are in the element vec (index)
    unsigned int scanPosition = 0;

    // This takes values from operationState::opLevels - represents which operations we're currently concerned with.
    std::vector<char> executionLevel;
};

template<class Ty>
const std::vector<std::vector<char>> operationState<Ty>::opLevels = {
//    {'(', ')'},
    {'^'},
    {'*', '/'},
    {'+', '-'},
};


// HACK: this is a hack for ensuring that the static member Element::operatorSet isn't multiply defined
class Element {
public:
    virtual std::string toString() {
        return "Element{}";
    };

    virtual float call(operationState<Element>* opState){
        std::cout << "[POTENTIAL ERROR]: Someone tried to call a base class element.\n";
        return 0.0f;
    };
};

// Stores numeric values, currently only as floats.
class NumericElement : public Element {
private:
    float _data;
public:
    NumericElement(float source) : _data(source) {};

    std::string toString() override {
        std::stringstream ss;
        ss << "NumericElement{" << _data << "}";
        return ss.str();
    };

    float call(operationState<Element>* opState) override {
        return _data;
    };
};

// The Element type that represents an operator (+, -, ^, sin()) in our function. Currently only does one character ops.
class OperatorElement : public Element {
private:
    char _data;
public:
    OperatorElement(char source) : _data(source) {};

    std::string toString() override {
        std::stringstream ss;
        ss << "OperatorElement{" << _data << "}";
        return ss.str();
    };

    // TODO: Gotta get cleaned up.
    float call(operationState<Element>* opState) override {
        if(std::find(opState->executionLevel.begin(), opState->executionLevel.end(), _data) != opState->executionLevel.end()){
            // If we're on the execution level we're concerned with.
            switch(_data){
                case '+':
                {
                    auto val = opState->data[opState->scanPosition-1]->call(opState) + opState->data[opState->scanPosition+1]->call(opState);
					// How does this work if scanPosition = 0?
                    opState->data.erase(opState->data.begin() + opState->scanPosition-1, opState->data.begin() + opState->scanPosition+2);
					if (opState->scanPosition <= 1) {
						// This has to be either an exempt call (by either an outside source or another element) or an operator at the start of a func, hence we need to handle insertion differently.
						opState->data.insert(opState->data.begin(), std::make_shared<NumericElement>(val));
						opState->scanPosition -= 1;
					}
					else {
						opState->scanPosition -= 1;
						// deleted - 1
						opState->data.insert(opState->data.begin() + opState->scanPosition, std::make_shared<NumericElement>(val));
						opState->scanPosition -= 1;
					}
                    return val;
                    break;
                }
                case '-':
                {
                    auto val = opState->data[opState->scanPosition-1]->call(opState) - opState->data[opState->scanPosition+1]->call(opState);
                    opState->data.erase(opState->data.begin() + opState->scanPosition-1, opState->data.begin() + opState->scanPosition+2);
					if (opState->scanPosition <= 1) {
						// This has to be either an exempt call (by either an outside source or another element) or an operator at the start of a func, hence we need to handle insertion differently.
						opState->data.insert(opState->data.begin(), std::make_shared<NumericElement>(val));
						opState->scanPosition -= 1; // TODO: Does this work for scanPosition = 0?
					}
					else {
						opState->scanPosition -= 1;
						// deleted - 1
						opState->data.insert(opState->data.begin() + opState->scanPosition, std::make_shared<NumericElement>(val));
						opState->scanPosition -= 1;
					}
                    return val;
                    break;
                }
                case '*':
                {
                    auto val = opState->data[opState->scanPosition-1]->call(opState) * opState->data[opState->scanPosition+1]->call(opState);
                    opState->data.erase(opState->data.begin() + opState->scanPosition-1, opState->data.begin() + opState->scanPosition+2);
					if (opState->scanPosition <= 1) {
						// This has to be either an exempt call (by either an outside source or another element) or an operator at the start of a func, hence we need to handle insertion differently.
						opState->data.insert(opState->data.begin(), std::make_shared<NumericElement>(val));
						opState->scanPosition -= 1; // TODO: Does this work for scanPosition = 0?
					}
					else {
						opState->scanPosition -= 1;
						// deleted - 1
						opState->data.insert(opState->data.begin() + opState->scanPosition, std::make_shared<NumericElement>(val));
						opState->scanPosition -= 1;
					}
                    return val;
                    break;
                }
                case '/':
                {
                    auto val = opState->data[opState->scanPosition-1]->call(opState) / opState->data[opState->scanPosition+1]->call(opState);
                    opState->data.erase(opState->data.begin() + opState->scanPosition-1, opState->data.begin() + opState->scanPosition+2);
					if (opState->scanPosition <= 1) {
						// This has to be either an exempt call (by either an outside source or another element) or an operator at the start of a func, hence we need to handle insertion differently.
						opState->data.insert(opState->data.begin(), std::make_shared<NumericElement>(val));
						opState->scanPosition -= 1; // TODO: Does this work for scanPosition = 0?
					}
					else {
						opState->scanPosition -= 1;
						// deleted - 1
						opState->data.insert(opState->data.begin() + opState->scanPosition, std::make_shared<NumericElement>(val));
						opState->scanPosition -= 1;
					}
                    return val;
                    break;
                }
                case '^':
                {
                    auto val = pow(opState->data[opState->scanPosition-1]->call(opState), opState->data[opState->scanPosition+1]->call(opState));
                    opState->data.erase(opState->data.begin() + opState->scanPosition-1, opState->data.begin() + opState->scanPosition+2);
					if (opState->scanPosition <= 1) {
						// This has to be either an exempt call (by either an outside source or another element) or an operator at the start of a func, hence we need to handle insertion differently.
						opState->data.insert(opState->data.begin(), std::make_shared<NumericElement>(val));
						opState->scanPosition -= 1; // TODO: Does this work for scanPosition = 0?
					}
					else {
						opState->scanPosition -= 1;
						// deleted - 1
						opState->data.insert(opState->data.begin() + opState->scanPosition, std::make_shared<NumericElement>(val));
						opState->scanPosition -= 1;
					}
                    return val;
                    break;
                }
                default:
                    std::cout << "[EXECUTION ERROR]: [ELEMENT.CALL]: The operator that was called on, {" << _data << "}, was found in the operation state's execution level, but the Element didn't implement it.\n";
                    return 0.0f;
                    break;
            }
        }
        // TODO: Is this an error? Should we carry out the operation without changing the opState->data?
        // std::cout << "[EXECUTION ERROR]: [ELEMENT.CALL]: The operator that was called on, {" << _data << "}, at scan {" << opState->scanPosition << "} was not found in the operation state's execution level.\n";
        return 0.0f;
    };
};

// Represents any constants or variables in a Function. Takes values from opState->variableValues.
class VariableElement : public Element {
private:
    char _data;
public:
    VariableElement(char source) : _data(source) {};

    std::string toString() override {
        std::stringstream ss;
        ss << "VariableElement{" << _data << "}";
        return ss.str();
    }

    float call(operationState<Element>* opState) override {
        if(opState->variableValues.find(_data) != opState->variableValues.end()){
            return opState->variableValues[_data];
        }
        else {
            std::cout << "[VariableElement::call]: An unassigned (no value given in Function::call()) variable was queried for value.\n";
            return 0.0f;
        }
    }
};
