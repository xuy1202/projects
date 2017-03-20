/*
 * Given a string s consists of upper/lower-case alphabets and empty space characters ' ', return the length of last word in the string.
 *
 * If the last word does not exist, return 0.
 *
 * Note: A word is defined as a character sequence consists of non-space characters only.
 *
 * For example,
 * Given s = "Hello World",
 * return 5. 
 */

#include<iostream>


class Solution {
    public:
        int lengthOfLastWord(const char *s) {
            const char* c = s;
            int count = 0;
            int lastc = 0;
            while(*c){
                if(*c == ' '){
                    lastc = count>0? count: lastc;
                    count = 0;
                }else{
                    ++count;
                }
                ++c;
            }
            lastc = count>0? count: lastc;
            return lastc;
        }
};


int main(void)
{
    std::cout << Solution().lengthOfLastWord("Hello World") << std::endl;
    std::cout << Solution().lengthOfLastWord("Hello World ") << std::endl;
    std::cout << Solution().lengthOfLastWord("Hello World  ") << std::endl;
    std::cout << Solution().lengthOfLastWord("") << std::endl;
    std::cout << Solution().lengthOfLastWord(" ") << std::endl;
    std::cout << Solution().lengthOfLastWord("a") << std::endl;
    return 0;
}


