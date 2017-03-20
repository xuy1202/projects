/*
 * Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
 *
 * The brackets must close in the correct order, "()" and "()[]{}" are all valid but "(]" and "([)]" are not.
 */

#include <iostream>
#include <vector>
#include <map>


class Solution {
    public:
        bool isValid(std::string s) {
            std::vector<char> stack;
            for(auto it=s.begin(); it!=s.end(); ++it){
                switch(*it){
                    case '{':
                        stack.push_back('{'); break;
                    case '[':
                        stack.push_back('['); break;
                    case '(':
                        stack.push_back('('); break;

                    case '}':
                        if(stack.empty())          return false;
                        if(*stack.rbegin() != '{') return false;
                        stack.pop_back();                 break;
                    case ']':
                        if(stack.empty())          return false;
                        if(*stack.rbegin() != '[') return false;
                        stack.pop_back();                 break;
                    case ')':
                        if(stack.empty())          return false;
                        if(*stack.rbegin() != '(') return false;
                        stack.pop_back();                 break;
                    default:
                        return false;
                }
            }
            return stack.empty();
        }
};


int main(void)
{
    //* The brackets must close in the correct order, "()" and "()[]{}" are all valid but "(]" and "([)]" are not.
    std::cout << Solution().isValid("{}") << std::endl;
    std::cout << Solution().isValid("()") << std::endl;
    std::cout << Solution().isValid("[]") << std::endl;
    std::cout << Solution().isValid("(){}[]") << std::endl;
    std::cout << Solution().isValid("(]") << std::endl;
    std::cout << Solution().isValid("([)]") << std::endl;
    std::cout << Solution().isValid("") << std::endl;
    return 0;
}

