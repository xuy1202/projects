
#include "helper.h"


class Solution {
    public:
        inline bool is_vowel(unsigned char c)
        {
            switch(c){
                case 'a':
                case 'e':
                case 'i':
                case 'o':
                case 'u':
                case 'A':
                case 'E':
                case 'I':
                case 'O':
                case 'U':
                    return true;
                default:
                    return false;
            }
        }

        std::string reverseVowels(std::string s){
            if(s.size() <= 1) return s;
            auto bp = 0;
            auto ep = s.size() - 1;
            while(bp < ep){
                if(! is_vowel(s[bp])){ ++bp; continue; }
                if(! is_vowel(s[ep])){ --ep; continue; }
                //std::cout << bp << "|" << s[bp] << "\t" << ep << "|" << s[ep] << std::endl;
                unsigned char tmp = s[ep];
                s[ep] = s[bp];
                s[bp] = tmp;
                ++bp; --ep;
            }
            return s;
        }
};

int main()
{
    std::cout << Solution().reverseVowels("hello") << std::endl;
    std::cout << Solution().reverseVowels("leetcode") << std::endl;
    std::cout << Solution().reverseVowels("a.") << std::endl;
    std::cout << Solution().reverseVowels("ai") << std::endl;
    std::cout << Solution().reverseVowels("Marge, let's \"went.\" I await news telegram.") << std::endl;

    return 0;
}

