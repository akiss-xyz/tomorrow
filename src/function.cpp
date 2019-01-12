#include "function.hpp"

Function::Function(std::string operation){
    constOp = operation;

    // TODO: Sort log

    if(_debugLevel >= logLevels::TRACE){
        std::cout << "[FUNCTION]: Init w/ constOp{" << constOp << "}\n";
    }
}

void Function::plot(Image* img){
    for(int i = 0; i < (int)img->xSize; i++){
        if(i >= (int)(img->xSize/2)){
            if(_debugLevel >= logLevels::SCAN){
                std::cout << "[--- PLOT ---]: Calling " << constOp << " at i = {" << i << "} with input: {" << i-(int)(img->xSize/2) << "}, output was {";
            }

            float val = call((int)i-(img->xSize/2));
            std::cout << val << "}\n";
            unsigned char randomColour[3] = { 255, 255, 255 };
            img->image->draw_point(i, img->ySize/2 - (int)std::round(val), randomColour, 1);
        }
    }
}

void Function::plot(Image* img, unsigned char* colour){
    for(int i = 0; i < (int)img->xSize; i++){
        if(i > (int)(img->xSize/2)){
            if(_debugLevel >= logLevels::SCAN){
                std::cout << "[--- PLOT ---]: Calling " << constOp << " at i = {" << i << "} with input: {" << i-(int)(img->xSize/2) << "}, output was {";
            }

            float val = call((int)i-(img->xSize/2));
            std::cout << val << "}\n";
            img->image->draw_point(i, img->ySize/2 - (int)std::round(val), colour, 1);
        }
    }
}


float Function::call(float x){
    // TODO: Comment properly
    op = constOp;

    for(unsigned int key = 0; key < Keys.size(); key++){
        // TODO: Logging fix
        // std::cout << "[NEW KEY]: Current key [" << key << "], with char {" << Keys[key].first << "}\n";

        for(unsigned int i = 0; i < op.length(); i++){
            if(op[i] == Keys[key].first){
                // Calculate its value and replace the term with its value
                // TODO: Make this work - even though it does the maths, a lot of the string formatting business breaks.
                // Addition and subtraction do not format properly, as seen in op 2

                // TODO: Clean this up & Comment properly.

                /// TODO: Make this work off of the length of the string obtained by the ReadNum functions.
                int leftOffset = 0;
                int rightOffset = 0;

                float val = Keys[key].second(ReadNumBack(i, &leftOffset, x), ReadNum(i, &rightOffset, x));

                if(_debugLevel >= logLevels::SCAN){
                    std::cout << "[CALC]: Obtained value: {" << val << "} from operation: {" << op[i] << "} at {" << i << "}\n" <<
                            "[CALC]: Operands were: Left:{" << ReadNumBack(i, nullptr, x) << "}, Right:{" << ReadNum(i, nullptr, x) << "} LO, RO: {" << leftOffset << "," << rightOffset << "}\n";
                }

                int startVal = (signed int)i - leftOffset;
                int lengthToCut = leftOffset + rightOffset + 1;

                op.erase(startVal, lengthToCut);            // Remove the term.
                op.insert(startVal, std::to_string(val));   // Add its value in its place.

                if(_debugLevel >= logLevels::TRACE){
                    std::cout << "[UPDATED OP]:{" << op << "}\n"; 
                }
                
                i = 0; // Reset iterator so we don't skip over characters
            }
        }
    }
    return std::stof(op);
}

float Function::ReadNum(unsigned int pos, int* length, float x) noexcept {
    pos += 1; 
    std::string numBuffer; bool isNegative = false;

    // TODO: Read up on the different casts and make sure we're doing this correctly - don't wanna accidentally not follow best practices in C++ ;3
    for(unsigned int i = pos; i < (unsigned int)op.length(); i++){
        if(48 <= (int)op[i] && (int)op[i] <= 57){  // Is a number
            if(length){ (*length)++;}
            
            numBuffer.push_back(op[i]);
        }
        else if(op[i] == 'x' || op[i] == 'X'){
            op.erase(i, 1);
            op.insert(i, std::to_string(x));
            i--; // Do this again so we don't skip any chars
        }
        else if(op[i] == 'n' && i == 0){
            isNegative = true;
        }
        else if(op[i] == *"."){  // TODO: Test this // This is dumb what the hell
            numBuffer.push_back(op[i]);
        }
        else {    // NaN
            if(isNegative){
                return -1 * std::stof(numBuffer);
            }
            return std::stof(numBuffer);
        }
    }
    return -1.0f;
}

float Function::ReadNumBack(int pos, int* length, float x) noexcept {
    std::string numBuffer; bool isNegative = false;
    pos -= 1;
    for(int i = pos; i >= 0; i--){
        // TODO: Read up on the different casts and make sure we're doing this correctly - don't wanna accidentally not follow best practices in C++ ;3
        if(48 <= (int)op[i] && (int)op[i] <= 57){  // Is a number
            if(length){ (*length)++;}
            numBuffer.push_back(op[i]);
        } 
        else if(op[i] == 'x' || op[i] == 'X'){
            op.erase(i, 1);
            op.insert(i, std::to_string(x));
        }
        
        else if(op[i] == *"."){  // TODO: Test this
            numBuffer.push_back(op[i]);
        }
        else {    // NaN
            if(op[i] == 'n'){
                isNegative = true;
            }
            reverseStr(numBuffer);
            if(isNegative){
                length++;
                return -1 * std::stof(numBuffer);
            }
            return std::stof(numBuffer);
        }
    }
    reverseStr(numBuffer);
    return std::stof(numBuffer);
}

inline void Function::reverseStr(std::string& str) const noexcept { 
    int n = str.length();
    for (int i = 0; i < n / 2; i++){
        std::swap(str[i], str[n - i - 1]); 
    }
}

inline void Function::setOperation(std::string newOp){
    this->op = newOp;
}
