/*
 * Given a positive integer, return its corresponding column title as appear in an Excel sheet.
 *
 * For example:
 *
 *    1 -> A
 *    2 -> B
 *    3 -> C
 *    ...
 *    26 -> Z
 *    27 -> AA
 *    28 -> AB 
 */
#include "helper.h"

class Solution {
    public:
        std::string convertToTitle(int n){
            std::string ret = "";
            while(n > 26){
                int rem = n%26;
                n /= 26;
                if(rem == 0){
                    ret = "Z" + ret;
                    n -= 1;
                }else{
                    ret = std::string(1, 'A' + rem - 1) + ret;
                }
            }
            ret = std::string(1, 'A' + n - 1) + ret;
            return ret;
        }
};


int main(void)
{
    std::cout << Solution().convertToTitle(1) << std::endl;
    std::cout << Solution().convertToTitle(2) << std::endl;
    std::cout << Solution().convertToTitle(3) << std::endl;
    std::cout << Solution().convertToTitle(26) << std::endl;
    std::cout << Solution().convertToTitle(27) << std::endl;
    std::cout << Solution().convertToTitle(28) << std::endl;
    std::cout << Solution().convertToTitle(52) << std::endl;
    return 0;
}


