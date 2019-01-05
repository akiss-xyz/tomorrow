#include <iostream> // Input and output streams. This allows us to talk to the terminal, basically. This shit is a huge file btw holy fuck
#include <string>

class Function{
public:
    void call(){
        
        // Brackets
            // Search for pairs of ()
            // Make new function objects out of those and call them

        // Powers
            // Search for ^
            // Do the jab
        
        sin() -> sinFunc
        std::pair<string, std::function<>> key = std::make_pair("(", [](){
            blah blah
        }; // what we're searching for, and the thing to do with it
        // Division
            // Search for /

        // Multiply
            // Search for *
        for(int i = 0; i < 11; i++){
            switch(op[i]){
                case '*':
                    float left = ReadNumBack(i);
                    std::cout << "LEFT OF MULTPIPLY:" << left << '\n';
                    float right = ReadNum(i);
                    std::cout << "RIGHT OF MULTPIPLY:" << right << '\n';

                    val = left + right;
                    std::cout << "VALUE: " << (int)val;
            }
        }
    }

    // TODO: Can't handle 70.65
    float ReadNum(int pos){
        pos += 1;
        std::string numBuffer;
        for(int i = pos; i < 12; i++){
            if(48 <= (int)op[i] && (int)op[i] <= 57){  // Is a number
                numBuffer.push_back(op[i]);
                std::cout << "RN: Character at: " << i << ": " << op[i] << '\n';
            } 
            else if(op[i] == *"."){  // TODO: Test this // This is dumb what the hell
                numBuffer.push_back(op[i]);
            }
            else {    // NaN
                return std::stof(numBuffer);
            }
        }
        return -1.0f;
    }

    

    float ReadNumBack(int pos){
        std::string numBuffer; 
        pos -= 1;

        for(int i = pos; i >= 0; i--){
            std::cout << "RNB: Character at: " << i << ": " << op[i] << '\n';
            if(48 <= (int)op[i] && (int)op[i] <= 57){  // Is a number
                numBuffer.push_back(op[i]);
            } 
            else if(op[i] == *"."){  // TODO: Test this
                numBuffer.push_back(op[i]);
            }
            else {    // NaN
                std::cout << "in readnumback: " << numBuffer << '\n';
                reverseStr(numBuffer);
                std::cout << "in readnumback\n";
                return std::stof(numBuffer);
            }
        }
        reverseStr(numBuffer);
        return std::stof(numBuffer);
    }

    void reverseStr(std::string& str) 
    { 
        int n = str.length(); 
        std::cout << "Length of str:" << n << '\n';
    
        for (int i = 0; i < n / 2; i++) 
            std::swap(str[i], str[n - i - 1]); 
    } 

    std::string op = "595959*1937*7,"; // -> "18372712483*7" -> put back in function class -> "857327581098"
    float val = 0;
};

int main()
{
    Function f; Function g;
    
    f.call();
    return EXIT_SUCCESS;
}

// Numbers range: 48 - 57 inclusive
