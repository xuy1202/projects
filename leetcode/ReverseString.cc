
#include "helper.h"

class Solution {
    public:
        std::string reverseString(std::string s) {
            auto len = s.size();
            auto mid = s.size()/2;
            for(size_t i=0; i<mid; ++i){
                auto chr = s[i];
                s[i] = s[len-i-1];
                s[len-i-1] = chr;
            }
            return s;
        }
};


int main()
{
    std::string s1 = "abc";
    std::cout << Solution().reverseString(s1) << std::endl;
    std::cout << Solution().reverseString("abcd") << std::endl;

    return 0;
}
