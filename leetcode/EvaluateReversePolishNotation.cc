/*
 *  Evaluate the value of an arithmetic expression in Reverse Polish Notation.
 *
 *  Valid operators are +, -, *, /. Each operand may be an integer or another expression.
 *
 *  Some examples:
 *
 *    ["2", "1", "+", "3", "*"] -> ((2 + 1) * 3) -> 9
 *    ["4", "13", "5", "/", "+"] -> (4 + (13 / 5)) -> 6
 */

#include "helper.h"


class Solution {
    public:
#define OrganizeStack_(op)         \
        int r = *(stack.rbegin()); \
        stack.pop_back();          \
        int l = *(stack.rbegin()); \
        stack.pop_back();          \
        stack.push_back(l op r);

        int evalRPN(std::vector<std::string> &tokens) {
            std::vector<int> stack;
            for(auto it=tokens.begin(); it!=tokens.end(); ++it){
                const std::string s = *it;
                if(s == "+"){
                    OrganizeStack_(+);
                }else if(s == "-"){
                    OrganizeStack_(-);
                }else if(s == "*"){
                    OrganizeStack_(*);
                }else if(s == "/"){
                    OrganizeStack_(/);
                }else{
                    stack.push_back(atoi(s.c_str()));
                }
            }
            return stack.empty()? 0: *(stack.rbegin());
        }
#undef OrganizeStack_
};


int main(void)
{
    std::vector<std::string> tokens = {"2", "1", "+", "3", "*"};
    std::cout << Solution().evalRPN(tokens) << std::endl;

    std::vector<std::string> tokens2 = {"4", "13", "5", "/", "+"};
    std::cout << Solution().evalRPN(tokens2) << std::endl;

    return 0;
}


