#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<limits.h>


class Solution {
    public:
        int atoi(const char *str) {
            auto i = 0;
            // find the first non-space char
            while(*(str+i) == ' '){
                ++i;
                continue;
            }
            // judge if negative
            bool neg = false;
            switch(*(str+i)){
                case '-': ++i; neg = true ; break;
                case '+': ++i; neg = false; break;
                default :      neg = false; break;
            }

            int ret = 0;
            while(true){
                char c = *(str + i++);
                if(! std::isdigit(c)){
                    break;
                }
                // using negative number to cal
                if(ret == 0 && c !='0'){
                    ret = '0' - c;
                    continue;
                }
                // over flow checking
                int tmp = ret * 10 - (c - '0');
                if(tmp > 0 || (tmp + c - '0')/10 != ret){
                    return (neg? INT_MIN: INT_MAX);
                }
                ret = tmp;
            }
            return neg? ret: ((ret == INT_MIN)? INT_MAX: -ret);
        }
};


int main(void)
{
    std::cout << "1:"  << (Solution().atoi("1")             == 1          ) << std::endl;
    std::cout << "2:"  << (Solution().atoi("123")           == 123        ) << std::endl;
    std::cout << "3:"  << (Solution().atoi("+123")          == 123        ) << std::endl;
    std::cout << "4:"  << (Solution().atoi("-123")          == -123       ) << std::endl;
    std::cout << "5:"  << (Solution().atoi("-999999999")    == -999999999 ) << std::endl;
    std::cout << "6:"  << (Solution().atoi("999999999")     == 999999999  ) << std::endl;
    std::cout << "7:"  << (Solution().atoi("9999999991")    == INT_MAX    ) << std::endl;
    std::cout << "7:"  << (Solution().atoi("-9999999991")   == INT_MIN    ) << std::endl;
    std::cout << "8:"  << (Solution().atoi("    010")       == 10         ) << std::endl;
    std::cout << "9:"  << (Solution().atoi("     -004500")  == -4500      ) << std::endl;
    std::cout << "10:" << (Solution().atoi("2147483648")    == 2147483647 ) << std::endl;
    std::cout << "11:" << (Solution().atoi("-2147483647")   == -INT_MAX   ) << std::endl;
    std::cout << "12:" << (Solution().atoi("-2147483648")   == INT_MIN    ) << std::endl;
    return 0;
}

