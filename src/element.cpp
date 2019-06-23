#include "element.hpp"

#include "function.hpp"

const std::vector<std::vector<std::string>> Element::operationState::opLevels = {
    //    {'(', ')'},
    {"^"},
    {"*", "/"},
    {"+", "-"},
};

std::map<std::string, std::function<float(float, float)>> Element::operationState::signatureToFuncMap = {
    {"+", [](float lhs, float rhs) { return lhs + rhs; }},
    {"-", [](float lhs, float rhs) { return lhs - rhs; }},
    {"*", [](float lhs, float rhs) { return lhs * rhs; }},
    {"/", [](float lhs, float rhs) { return lhs / rhs; }},
    {"^", [](float lhs, float rhs) { return pow(lhs, rhs); }},
    {"sin", [](float lhs, float rhs) { return sin(rhs); }},
    {"cos", [](float lhs, float rhs) { return cos(rhs); }},
    {"tan", [](float lhs, float rhs) { return tan(rhs); }},
};

/* Element methods */

std::string Element::toString() const noexcept
{
    return "Element{}";
};

char Element::getType() const noexcept
{
    return 'e';
};

float Element::call(operationState *opState, unsigned int selfPos) const noexcept
{
    std::cout << "[POTENTIAL ERROR]: Someone tried to call a base class element.\n";
    return 0.0f;
};

float Element::binaryOperation(float val, operationState *opState) noexcept
{
    // How does this work if scanPosition = 0?
    opState->data.erase(opState->data.begin() + opState->scanPosition - 1, opState->data.begin() + opState->scanPosition + 2);
    if (opState->scanPosition <= 1)
    {
        // This has to be either an exempt call (by either an outside source or another element) or an operator at the start of a func, hence we need to handle insertion differently.
        opState->data.insert(opState->data.begin(), std::make_shared<NumericElement>(val));
        opState->scanPosition -= 1;
    }
    else
    {
        opState->scanPosition -= 1;
        // deleted - 1
        opState->data.insert(opState->data.begin() + opState->scanPosition, std::make_shared<NumericElement>(val));
        opState->scanPosition -= 1;
    }
    opState->isValueCall = false;
    return val;
};

float Element::unaryOperation(float val, operationState *opState) noexcept
{
    return val;
};

/* NumericElement methods */

std::string NumericElement::toString() const noexcept
{
    std::stringstream ss;
    ss << "NumericElement{" << _data << "}";
    return ss.str();
};

char NumericElement::getType() const noexcept
{
    return 'n';
};

float NumericElement::call(operationState *opState, unsigned int selfPos) const noexcept
{
    return _data;
};

/* OperatorElement methods */

std::string OperatorElement::toString() const noexcept
{
    std::stringstream ss;
    ss << "OperatorElement{" << _data << "}";
    return ss.str();
};

char OperatorElement::getType() const noexcept
{
    if(_data.length() == 1){ // TODO: Have a way to tell whether an operator is unary or binary.
        return 'o';
    }
    return 'u';
};

float OperatorElement::call(operationState *opState, unsigned int selfPos) const noexcept
{
 //   if (std::find(opState->executionLevel.begin(), opState->executionLevel.end(), _data) != opState->executionLevel.end())
    // std::cout << "Calling opElem (" << _data << ") scanPos is ," << opState->scanPosition << ", selfPos is " << selfPos << "\n";
    if (auto func = operationState::signatureToFuncMap.at(_data); func)
    {
        opState->isValueCall = true;
        float value;
        if (selfPos == 0 || _data.length() != 1)
        {
            value = func(
                0.0f,
                opState->data.at(selfPos + 1)->call(opState, selfPos + 1) // Value of rhs
            );
        }
        else
        {
            value = func(
                opState->data.at(selfPos - 1)->call(opState, selfPos - 1), // Value of lhs
                opState->data.at(selfPos + 1)->call(opState, selfPos + 1)  // Value of rhs
            );
        }
        if (_data.length() == 1)
        {
            // std::cout << "...returned binaryOp = " << unaryOperation(value, opState) << "\n";
            return binaryOperation(value, opState);
        }
        else
        {
            // std::cout << "...returned unaryOp = " << unaryOperation(value, opState) << "\n";
            return unaryOperation(value, opState);
        }

        /*
        // If we're on the execution level we're concerned with.
        switch(_data){
            case '+':
            {
                auto val = opState->data.at(opState->scanPosition-1)->call(opState) + opState->data.at(opState->scanPosition+1)->call(opState);
                return binaryOperation(val, opState);
                break;
            }
            case '-':
            {
                auto val = opState->data.at(opState->scanPosition-1)->call(opState) - opState->data.at(opState->scanPosition+1)->call(opState);
                return binaryOperation(val, opState);
                break;
            }
            case '*':
            {
                auto val = opState->data.at(opState->scanPosition-1)->call(opState) * opState->data.at(opState->scanPosition+1)->call(opState);
                return binaryOperation(val, opState);
                break;
            }
            case '/':
            {
                auto val = opState->data.at(opState->scanPosition-1)->call(opState) / opState->data.at(opState->scanPosition+1)->call(opState);
               return binaryOperation(val, opState);
                break;
            }
            case '^':
            {
                auto val = pow(opState->data.at(opState->scanPosition-1)->call(opState), opState->data.at(opState->scanPosition+1)->call(opState));
                return binaryOperation(val, opState);
                break;
            }
            default:
                std::cout << "[EXECUTION ERROR]: [ELEMENT.CALL]: The operator that was called on, {" << _data << "}, was found in the operation state's execution level, but the Element didn't implement it.\n";
                return 0.0f;
                break;
        }
        */
    }
    // TODO: Is this an error? Should we carry out the operation without changing the opState->data?
    // Testing on f(x) = (x^2)*(x^2), this works but complains. Muting for now.
    // std::cout << "[EXECUTION ERROR]: [ELEMENT.CALL]: The operator that was called on, {" << _data << "}, at scan {" << opState->scanPosition << "} was not found in the operation state's execution level.\n";
    return 0.0f;
};

/* VariableElement methods */

std::string VariableElement::toString() const noexcept
{
    std::stringstream ss;
    if (_sign)
    {
        ss << "VariableElement{" << _data << "}";
    }
    else
    {
        ss << "VariableElement{-" << _data << "}";
    }
    return ss.str();
};

char VariableElement::getType() const noexcept
{
    return 'v';
};

float VariableElement::call(operationState *opState, unsigned int selfPos) const noexcept
{
    if (opState->variableValues.find(_data) != opState->variableValues.end())
    {
        if (_sign)
        {
            return opState->variableValues[_data];
        }
        else
        {
            return -1 * opState->variableValues[_data];
        }
    }
    else
    {
        // std::cout << "[VariableElement::call]: An unassigned (no value given in Function::call()) variable was queried for value.\n";
        return 0.0f;
    }
};

/* BracketElement methods */

std::string BracketElement::toString() const noexcept
{
    std::stringstream ss;
    ss << "BracketElement{" << _data->toString() << "}";
    return ss.str();
};

char BracketElement::getType() const noexcept
{
    return 'b';
};

float BracketElement::call(operationState *opState, unsigned int selfPos) const noexcept
{
    if (opState->isValueCall)
    {
        return _data->call(opState->variableValues);
    }
    return 0.0f;
};
