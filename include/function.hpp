#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <iostream>
#include <map>

#include "element.hpp"

class Function {
public:
    // Just to help us get what's happening.
    void printOpState(const operationState<Element>* const opState, bool all){
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
    }

    // Deals with 
    static const unsigned int nOfOperators = 6;
    static const char operatorSet[6];

    std::string toString(){
        std::stringstream ss;
        ss << "Function elemtree:{";
        for(auto e : this->_data){
            ss << "e{" << e->toString() << "},";
        }
        ss << "}"; return ss.str();
    };

    Function(const std::string& source){
        for(unsigned int i = 0; i < source.length(); i++){
            auto elementContainer = getElementAt(i, source);
            if(elementContainer.successfulParse == true){
                this->_data.push_back(elementContainer.elem);
                i += elementContainer.elemLength - 1;
            } 
            else {
                std::cout << "[FUNCTION::FUNCTION(" << source << ")]: Parse error.\n";
            }
        }
    };

    float call(std::map<char, float> varMap){
        operationState<Element> opState = { _data, varMap, 0, operationState<Element>::opLevels[0]};
        for(auto operationLevel : operationState<Element>::opLevels){
            opState.executionLevel = operationLevel;
            while(opState.scanPosition < (opState.data.size()-1)){
                // printOpState(&opState, true);
                opState.data[opState.scanPosition]->call(&opState);
                opState.scanPosition++;
            }
            opState.scanPosition = 0;
        }
        // std::cout << "END OPSTATE:\n";
        // printOpState(&opState, true);
        return opState.data[0]->call(&opState);
    }
private:
    

    struct elemContainer {
    public:
        bool successfulParse;
        std::shared_ptr<Element> elem;
        unsigned int elemLength = 1;

        elemContainer(bool isSuccessful, std::shared_ptr<Element> element, unsigned int elemLen)
        : successfulParse(isSuccessful), elem(element), elemLength(elemLen) {};
    };

    std::vector<std::shared_ptr<Element>> _data;

    elemContainer getElementAt(unsigned int pos, const std::string& source){
        if (isNumber(source[pos])){
            std::string buf;
            for(unsigned int i = pos; i < source.length(); i++){
                if(isNumber(source[i]) || source[i] == '.'){
                    buf.push_back(source[i]);
                } else {
                    break;
                }
            }
            return elemContainer(true, std::make_shared<NumericElement>(std::stof(buf)), buf.length());
        }
        else { // HACK: can't get if-init working
            elemContainer elem = getOperator(pos, source);
            if (elem.successfulParse == true){
                return elem;
            }
            else if (isVariable(pos, source)){
                return elemContainer(true, std::make_shared<VariableElement>(source[pos]), 1);
            }
            else {
                return elemContainer(false, std::make_shared<Element>(), 1);
            }
        }
    };

    static inline elemContainer getOperator(unsigned int pos, const std::string& source){
        for(unsigned int i = 0; i < Function::nOfOperators; i++){
            if(operatorSet[i] == source[pos]){
                return elemContainer(true, std::make_shared<OperatorElement>(source[pos]), 1);
            }
        }
        return elemContainer(false, std::make_shared<OperatorElement>('0'), 1);
    };

    static inline bool isNumber(const char val){
        if(48 <= (int)val && (int)val <= 57){
            return true;
        } else {
            return false;
        }
    };

    static inline bool isVariable(unsigned int pos, const std::string& source){
        if( !( (int)'a' <= (int)source[pos+1] && (int)source[pos+1] <= (int)'z' ) && ( (int)'a' <= (int)source[pos] && (int)source[pos] <= (int)'z' ) ){
            return true;
        } else {
            return false;
        }
    };

};

