/*
 * Given a column title as appear in an Excel sheet, return its corresponding column number.
 *
 * For example:
 *
 *    A -> 1
 *    B -> 2
 *    C -> 3
 *    ...
 *    Z -> 26
 *    AA -> 27
 *    AB -> 28 
 */
#include "helper.h"


class Solution {
    public:
        int titleToNumber(const std::string& s) {
            int ret = 0;
            for(auto it=s.begin(); it!=s.end(); ++it){
                ret *= 26;
                ret += (*it - 'A' + 1);
            }
            return ret;
        }
};


int main()
{
    std::cout << Solution().titleToNumber("A") << std::endl;
    std::cout << Solution().titleToNumber("B") << std::endl;
    std::cout << Solution().titleToNumber("C") << std::endl;
    std::cout << Solution().titleToNumber("Z") << std::endl;
    std::cout << Solution().titleToNumber("AA") << std::endl;
    std::cout << Solution().titleToNumber("AB") << std::endl;
    std::cout << Solution().titleToNumber("AZ") << std::endl;
    std::cout << Solution().titleToNumber("BA") << std::endl;
    return 0;
}


