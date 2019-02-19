#include "function.hpp"

const char Function::operatorSet[6] = { '+', '-', '*', '/', '^', '(' };

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


Function::Function(const std::string& source) noexcept {
    // Go through each char in the source,
    for(unsigned int i = 0; i < source.length(); i++){
        // Try to parse an element from that position,
        auto elementContainer = getElementAt(i, source);
        if(elementContainer.successfulParse == true){
            // If we successfully found an element, put it into our data buffer.
            this->_data.push_back(elementContainer.elem);
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

Function::elemContainer Function::getElementAt(unsigned int pos, const std::string& source) const noexcept {
    if (isNumber(source, pos)){
        std::string buf;
        for(unsigned int i = pos; i < source.length(); i++){
            if(isNumber(source, i) || source[i] == '.'){
                buf.push_back(source[i]);
            } else {
                break;
            }
        }
        return elemContainer(true, std::make_shared<NumericElement>(std::stof(buf)), buf.length());
    }
	else if (isVariable(pos, source)) {
		if (source[pos] == '-') {
			return elemContainer(true, std::make_shared<VariableElement>(source[pos + 1], 0), 2);
		}
		return elemContainer(true, std::make_shared<VariableElement>(source[pos], 1), 1);
	}
    else { // HACK: can't get if-init working
        elemContainer elem = getOperator(pos, source);
        if (elem.successfulParse == true){
            return elem;
        }
        else {
            return elemContainer(false, std::make_shared<Element>(), 1);
        }
    }
};

inline Function::elemContainer Function::getOperator(unsigned int pos, const std::string& source) noexcept {
    for(unsigned int i = 0; i < Function::nOfOperators; i++){
        if(operatorSet[i] == source[pos]){
            return elemContainer(true, std::make_shared<OperatorElement>(source[pos]), 1);
        }
    }
    return elemContainer(false, std::make_shared<OperatorElement>('0'), 1);
};

inline bool Function::isNumber(const std::string& source, unsigned int pos) noexcept {
	if (source[pos] == '-') {
		if (pos > 0) {
			if (!isNumber(source, pos-1)) {
				if (('a' <= source[pos+1]) && (source[pos+1] <= 'z')) {
					return false;
				}
				else {
					return true;
				}
			} 
			else {
				return false;
			}
		}
		else {
			if (('a' <= source[pos+1]) && (source[pos+1] <= 'z')) {
				return false;
			}
			else {
				return true;
			}
		}
	}
    if((48 <= (int)source[pos] && (int)source[pos] <= 57)){
		return true;
    } else {
        return false;
    }
};

inline bool Function::isVariable(unsigned int pos, const std::string& source) noexcept {
	if (source[pos] == '-' && !((int)'a' <= (int)source[pos + 2] && (int)source[pos + 2] <= (int)'z') && ((int)'a' <= (int)source[pos + 1] && (int)source[pos + 1] <= (int)'z')) {
		return true; // a case like 7*-x+4
	}
    if( !( (int)'a' <= (int)source[pos+1] && (int)source[pos+1] <= (int)'z' ) && ( (int)'a' <= (int)source[pos] && (int)source[pos] <= (int)'z' ) ){
        return true;
    } else {
        return false;
    }
};
