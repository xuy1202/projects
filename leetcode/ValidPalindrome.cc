/*
 * Given a string, determine if it is a palindrome, considering only alphanumeric characters and ignoring cases.
 *
 * For example,
 * "A man, a plan, a canal: Panama" is a palindrome.
 * "race a car" is not a palindrome.
 */

#include <iostream>
#include <vector>
#include <string.h>

class Solution {
    public:
        bool isPalindrome(const std::string& s) {
            if(s.size() <= 1) return true;

            size_t beg = 0;
            size_t end = s.size() - 1;

            while(beg <= end){
                char lh, rh;
                do{
                    lh = std::tolower(s[beg]);
                    if(std::isalpha(lh) || std::isdigit(lh)) break;
                    if(++beg >= end) return true;
                }while(1);
                do{
                    rh = std::tolower(s[end]);
                    if(std::isalpha(rh) || std::isdigit(rh)) break;
                    if(--end <= beg) return true;
                }while(1);

                //std::cout << lh << " " << rh << std::endl;
                if(lh != rh) return false;
                ++beg;
                --end;
            }
            return true;
        }

};


int main(void)
{
    //std::cout << Solution().isPalindrome("A man, a plan, a canal: Panama") << std::endl; 
    //std::cout << Solution().isPalindrome("race a car") << std::endl;
    //std::cout << Solution().isPalindrome("") << std::endl;
    //std::cout << Solution().isPalindrome("a") << std::endl;
    //std::cout << Solution().isPalindrome("1a2") << std::endl;
    //std::cout << Solution().isPalindrome(".,") << std::endl;
    std::cout << Solution().isPalindrome("a.") << std::endl;
    return 0;
}

