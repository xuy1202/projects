/*
 * Reverse digits of an integer.
 *
 * Example1: x = 123, return 321
 * Example2: x = -123, return -321
 */

#include<iostream>
#include<stdlib.h>
#include<stdio.h>


class Solution1 {
    public:
        int reverse(int x) {
            bool neg = x < 0;
            char buf[16] = {0};
            auto len = snprintf(buf, 16, "%d", abs(x));
            for(auto i=0; i<len/2; ++i){
                char* rh = buf + i;
                char* lh = buf + len - i - 1;
                *lh = (*lh) ^ (*rh);
                *rh = (*lh) ^ (*rh);
                *lh = (*lh) ^ (*rh);
            }
            if(neg){
                return -strtoul(buf, NULL, 10);
            }else{
                return strtoul(buf, NULL, 10);
            }
        }
};


class Solution {
    public:
        int reverse(int x) {
            bool di = x<0;
            int ret = 0;
            while(x){
                int rem = x%10;
                ret = ret * 10 + rem;
                // for overflow
                if(ret!=0 && (ret<0)!=di){
                    return 0;
                }
                x = x/10;
            }
            return ret;
        }
};


int main(void)
{
    std::cout << Solution().reverse(10) << std::endl;
    std::cout << Solution().reverse(-1) << std::endl;
    std::cout << Solution().reverse(123) << std::endl;
    std::cout << Solution().reverse(-123) << std::endl;
    std::cout << Solution().reverse(-100100) << std::endl;
    std::cout << Solution().reverse(1000000008) << std::endl;
    return 0;
}

