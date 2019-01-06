#include <iostream>
#include <string>
#include <functional>
#include <cmath>


// TODO: Add constructor and destructor
class Function{
public:
    /***     Public vars    ***/

    // The number of keys that our Function can deal with. Because in an array, the number of elements have to be known at compile time,
    // this is a constant integer.
    static const int nKeys = 7;
    // This is our system for handling different operations. Ask me to explain this, it's better explained 'live'. There's a lot of 
    // interesting, rich, complex shit here, so don't worry if you don't get it right now. Seriously, don't sweat it. You'll love it tho
    std::pair<char, std::function<float(float, float)> > Keys[nKeys] = {
        std::make_pair('(', [](float left, float right){
                // TODO: Handle a bracket instance:
                // Read in the string to corresponding endbracket
                // Create new func object of that string
                // Run func object
                // Return value of func object
                
                return left + right;
            }),
        std::make_pair('%', [](float left, float right){
                return fmod(left, right);
            }),
        std::make_pair('^', [](float left, float right){
                return pow(left,  right);
            }),
        std::make_pair('*', [](float left, float right){
                return left * right;
            }),
        std::make_pair('/', [](float left, float right){
                return left / right;
            }),
        std::make_pair('+', [](float left, float right){
                return left + right;
            }),
        std::make_pair('-', [](float left, float right){
                return left - right;
            }),
    };

private:
    /***     Private vars    ***/

    // The string representing our operation - TODO: Make this set up in the constructor.
    std::string op = "15%4*3/7/5*2/4^2,";           // Equivalent to (15 & 4)*3        5*2      | if multiplication is taken before division. This calculation shows, really well,
                                                    //               ----------  /  --------    | both what works and what needs some fixing up. Everything works except for the
                                                    //                   7             4^2      | string formatting at the start, as the 3 remains after the second step.
public:
    /***     Main public functions    ***/

  /// TODO: Add setOperation function, add on-the-fly keyset change.
  /* Calls the current function operation, using the current key
set. To change the operation, call setOperation(). It's currently not possible to change the keyset on-the-fly.
  * @param [in] float x : The value of the variable in the function operation in this call.
  * @return void
  * @see Keys, setOperation(), getOperation()
  */
     void call(){       // TODO: Comment properly
        // For each key we handle - keys must be organized in order of operations
        for(int key = 0; key < nKeys; key++){
            
            std::cout << "[NEW KEY]: Current key [" << key << "], with char {" << Keys[key].first << "}\n";

            // For each character in the operation
            for(unsigned int i = 0; i<op.length(); i++){
                // If the character matches the operator we're looking for e.g. if it's a multiply, or a divide
                if(op[i] == Keys[key].first){
                    // Calculate its value and replace the term with its value
                    // TODO: Make this work - even though it does the maths, a lot of the string formatting business breaks.
                    // Addition and subtraction do not format properly, as seen in op 2

                    // TODO: Clean this up & Comment properly.

                    /// TODO: Make this work off of the length of the string obtained by the ReadNum functions.
                    // Variables to keep track of how many characters to the left and the right each number takes up
                    // e.g. in the term 7569*319203
                    //                  I         I
                    //                  -   term  -
                    // The left offset is 4, because we have 4 characters to the left that we must delete, and the right offset is 6, because we have 6 chars to delete.
                    int leftOffset = 0;
                    int rightOffset = 0;

                    // The value of the calculation.
                    float val = Keys[key].second(ReadNumBack(i, &leftOffset), ReadNum(i, &rightOffset));


                    std::cout << "[CALC]: Obtained value: {" << val << "} from operation: {" << op[i] << "} at {" << i << "}\n" <<
                                "[CALC]: Operands were: Left:{" << ReadNumBack(i, nullptr) << "}, Right:{" << ReadNum(i, nullptr) << "}\n";
                    // The location of where our term starts from.
                    int startVal = (signed int)i - leftOffset;
                    // The length of our term - how many characters to remove.
                    int lengthToCut = leftOffset + rightOffset + 1;

                    op.erase(startVal, lengthToCut);            // Remove the term.
                    op.insert(startVal, std::to_string(val));   // Add its value in its place.

                    std::cout << "[UPDATE OP]:{" << op << "}\n"; 
                    // Reset the iterator - if we change the string we need to rescan it for other such operations.
                    // This is because, if we delete a large term, we might accidentally skip over another multiplication.
					i = 0;
                }
            }
        }
    }

    
    /* Read a single float value from the operation, to the right of the current position.
    * @param [in] unsigned int pos : The current position of the scan in the string. Should be on the operation that triggered the call.
    * @param [out] int* length : A pointer to an integer that will hold the length of the offset. Used for the deletion of the term we're dealing with.
    * @return float : The value parsed from the operation string.
    * @see call(), ReadNumBack()
    */
    float ReadNum(unsigned int pos, int* length) noexcept {
        pos += 1; 
        std::string numBuffer;

        // TODO: Read up on the different casts and make sure we're doing this correctly - don't wanna accidentally not follow best practices in C++ ;3
        for(unsigned int i = pos; i < (unsigned int)op.length(); i++){
            if(48 <= (int)op[i] && (int)op[i] <= 57){  // Is a number
                if(length){ (*length)++;}
                
                numBuffer.push_back(op[i]);
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


    // TODO: Instead of using int* length and then taking that as an offset, make it work off of the length of the std::string
    // TODO: Test and rework the positioning. e.g. pos += 1 is a hack, make that work better.
    /* Read a single float value from the operation, to the left of the current position.
    * @param [in] unsigned int pos : The current position of the scan in the string. Should be on the operation that triggered the call.
    * @param [out] int* length : A pointer to an integer that will hold the length of the offset. Used for the deletion of the term we're dealing with.
    * @return float : The value parsed from the operation string.
    * @see call(), ReadNum()
    */
    float ReadNumBack(int pos, int* length) noexcept {
        std::string numBuffer; 
        pos -= 1;

        for(int i = pos; i >= 0; i--){
            // TODO: Read up on the different casts and make sure we're doing this correctly - don't wanna accidentally not follow best practices in C++ ;3
            if(48 <= (int)op[i] && (int)op[i] <= 57){  // Is a number
                if(length){ (*length)++;}
                numBuffer.push_back(op[i]);
            } 
            else if(op[i] == *"."){  // TODO: Test this
                numBuffer.push_back(op[i]);
            }
            else {    // NaN
                reverseStr(numBuffer);
                return std::stof(numBuffer);
            }
        }
        reverseStr(numBuffer);
        return std::stof(numBuffer);
    }

    /* A quick algorithm to reverse a std::string
    * @param [in] std::string& str : A reference to the string to reverse.
    * @see ReadNumBack(), ReadNum()
    */
    inline void reverseStr(std::string& str) const noexcept { 
        int n = str.length();
        for (int i = 0; i < n / 2; i++){
            std::swap(str[i], str[n - i - 1]); 
        }
    } 
}; 

int main()
{
    Function f();
    f.call();
    return EXIT_SUCCESS;
}