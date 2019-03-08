#include "function.hpp"

static constexpr inline bool isDigit(char c) noexcept {
    if('0' <= c && c <= '9') { 
        return true;
    }
    else { 
        return false; 
    }
}

const std::map<const char, Function::operatorInfo> Function::operatorSet = { {'+', { true, false }}, {'-', { true, false }}, {'*', { true, false } }, {'/', { true, false }}, {'^', {true, false} }, {'(', { false, true } } };

void Function::printOpState(const Element::operationState* const opState, bool all) const noexcept {
    std::cout << "------------- BEGIN OPSTATE --------------\n";
    std::cout << "[opState]:\n";
    if(all){
        std::cout << "[opLevels]: \n";
        std::cout << "{";
        for(auto v : opState->opLevels){
            std::cout << "{ ";
            for(char c : v){
                std::cout << "'" << c << "',";
            }
            std::cout << "},\n";
        }

        std::cout << "[varValues]: \n";
        std::cout << "{";
        for(std::pair<char, float> v : opState->variableValues){
            std::cout << "{ '" << v.first << "': " << v.second << "},\n";
        }
    }
    if(opState->data[0]){
        std::cout << "[data]: \n";
        for(auto v : opState->data){
            if(v){
                std::cout << v->toString() << ",\n";
            }
            else {
                std::cout << "[FAULT]: VOID POINTER-TO-ELEMENT IN OPSTATE->DATA\n";
            }
        }
    }
    else {
        std::cout << "[FAULT]: VOID POINTER-TO-ELEMENT AS FIRST IN OPSTATE->DATA\n";
    }
    
    
    std::cout << "[scanPos]: {" << opState->scanPosition << "} out of {" << opState->data.size()-1 << "}\n";
    std::cout << "[executionLevel]: {\n";
    for(auto c : opState->executionLevel){
        std::cout << "{ ";
        std::cout << "'" << c << "',";
        std::cout << "},\n";
    }
    std::cout << "}}\n------------- END OPSTATE ----------------\n";
};

std::string Function::toString() const noexcept {
    std::stringstream ss;
    ss << "Function{";
    for(auto e : this->_data){
        ss << "e{" << e->toString() << "},";
    }
    ss << "}"; return ss.str();
};

struct parsedElem {
public:
    bool isOperatorType;
    bool isValueType;
};

Function::Function(const std::string& source) noexcept {
    // Go through each char in the source,
    elemContainer::parsedElem latest;
    for(unsigned int i = 0; i < source.length(); i++){
        // Try to parse an element from that position,
        auto elementContainer = getElementAt(i, source, latest);
        if(elementContainer.successfulParse == true){
            // If we successfully found an element, put it into our data buffer.
            this->_data.push_back(elementContainer.elem);
            std::cout << "[Function]: data buf looks like: ";
            if(_data[0]){
                std::cout << "[data]: \n";
                for(auto v : _data){
                if(v){
                    std::cout << v->toString() << ",\n";
                }
                else {
                    std::cout << "[FAULT]: VOID POINTER-TO-ELEMENT IN OPSTATE->DATA\n";
                }
            }
    }
    else {
        std::cout << "[FAULT]: VOID POINTER-TO-ELEMENT AS FIRST IN OPSTATE->DATA\n";
    }
            latest = elementContainer.latestElem;
            // Move back i so that the next character we look at is just after the one that parsed this elem.
            i += elementContainer.elemLength - 1;
        } 
        else {
            // Couldn't parse from char.
            std::cout << "[FUNCTION::FUNCTION(" << source << ")]: Parse error.\n";
        }
    }
};

float Function::call(std::map<char, float> varMap) const noexcept {
    Element::operationState opState = { _data, varMap, 0, Element::operationState::opLevels[0]};
    // Go through each operation level (aka go down BIDMAS),
    for(auto operationLevel : Element::operationState::opLevels){
        opState.executionLevel = operationLevel;
        // printOpState(&opState, false);
        // While we're scanning within the bounds of our list of elements,
        while(opState.scanPosition < (opState.data.size()-1)){
            // Call each element, so that the operators can carry out their corresponding actions.
            // printOpState(&opState, true);
            opState.data[opState.scanPosition]->call(&opState);
            opState.scanPosition++;
        }
        // Reset the scanPosition so that the next operationLevel starts at the start.
        opState.scanPosition = 0;
    }
    // std::cout << "END OPSTATE:\n";
    // printOpState(&opState, true);

    // Return the value of the final single element.
    return opState.data[0]->call(&opState);
};

Function::elemContainer Function::getElementAt(unsigned int pos, const std::string& source, elemContainer::parsedElem latest) const noexcept {
    if (isNumeric(source, pos, latest)){
        std::string buf;
        bool encounteredNum = false; unsigned int nOfNegatives = 0;
        for(unsigned int i = pos; i < source.length(); i++){
            if(isDigit(source[i])){
                encounteredNum = true;
                buf.push_back(source[i]);
            }
            if(!encounteredNum && source[i] == '-'){
                nOfNegatives++;
                buf.push_back(source[i]);
            }
            if(source[i] == '.'){
                buf.push_back(source[i]);
            } else {
                break;
            }
        }
        if(nOfNegatives){
            if(nOfNegatives % 2){
                // Odd number of negatives
                buf.erase(std::remove(buf.begin()+1, buf.end(), '-'), buf.end());
            }
            buf.erase(std::remove(buf.begin(), buf.end(), '-'), buf.end());
        }
        std::cout << "[ getElementAt(" << pos << ", " << source << ", latest): We're about to call stof on this buffer: {" << buf << "}\n";
        return elemContainer(true, std::make_shared<NumericElement>(std::stof(buf)), buf.length(), {false, true});
    }
    // Pass in end pointer.
	else if (isVariable(pos, source, latest)) {
		if (source[pos] == '-') {
			return elemContainer(true, std::make_shared<VariableElement>(source[pos + 1], 0), 2, {false, true});
		}
		return elemContainer(true, std::make_shared<VariableElement>(source[pos], 1), 1, {false, true});
	}
    else { // HACK: can't get if-init working
        elemContainer elem = getOperator(pos, source);
        if (elem.successfulParse == true){
            return elem;
        }
        else {
            return elemContainer(false, std::make_shared<Element>(), 1, {false, false});
        }
    }
};

inline Function::elemContainer Function::getOperator(unsigned int pos, const std::string& source) noexcept {
    for(unsigned int i = 0; i < Function::nOfOperators; i++){
        // If we this character is found in our opSet, and the context suggests it can be an operator
        std::cout << "[ getOperator(" << pos << "," << source << ") ]: Looping through nOfOperators, operatorSet.count(source[pos]) == {" << (operatorSet.count(source[pos])) << "}\n";
        if(operatorSet.count(source[pos])){
            return elemContainer(true, std::make_shared<OperatorElement>(source[pos]), 1, {true, false});
        }
    }
    return elemContainer(false, std::make_shared<OperatorElement>('0'), 1, {false, false});
};


// This function needs to decide if we parse this point into a NumericElement or not.
inline bool Function::isNumeric(const std::string& source, unsigned int start, elemContainer::parsedElem latest) noexcept {
    std::cout << "[ isNumeric(" << source << ", " << start << "): ";
    if(source[start] == '-'){
        if(latest.isOperatorType && !latest.isValueType){ // If there's an operator on the left that's not a bracket.
           std::cout << "isNumeric at source[start+1]: " << isNumeric(source, start+1, latest) << "\n"; 
           return isNumeric(source, start+1, latest); // Return whether or not the character on the right of the minus is a number.
        }
        else {
            if(start == 0){
                std::cout << "start is 0, return true\n";
                return true; // Latest doesn't have a value yet because we're the first thing.
            }
            std::cout << "Not an operator on the left and not at the start. False. \n";
            return false;
        }
    }
    std::cout << "A digit. True.\n";
    return isDigit(source[start]);
};

static inline unsigned int findNumberEnd(unsigned int start, const std::string& source) noexcept {
    unsigned int len = 0;
    for(unsigned int i = start; i < source.length(); i++){
        if(isDigit(source[i])){
            len++;
        } else {
            return start + len;
        }
    }
    // We got to the end of the string.
    return start+len;
};

static inline bool namesVar(char c) noexcept {
    if('a' <= c && c <= 'z'){ 
        return true;
    }
    else {
        return false;
    }
};

inline bool Function::isVariable(unsigned int pos, const std::string& source, elemContainer::parsedElem latest) noexcept {
    std::cout << "[ isVariable(" << pos << "," << source << ",{isOperatorType:" << latest.isOperatorType << ", isValueType: " << latest.isValueType << "}]: namesVar[source(pos+1)={" << namesVar(source[pos+1]) << "}\n";  
    if(source[pos] == '-' && namesVar(source[pos+1]) && !latest.isValueType){
        return true; // e.g. 7*-x
    } 
    if(namesVar(source[pos])){
        return true;
    } else {
        return false;
    }
};
