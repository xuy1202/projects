/*
 *  Given two binary strings, return their sum (also a binary string).
 *
 *  For example,
 *  a = "11"
 *  b = "1"
 *  Return "100". 
 */

#include <iostream>
#include <algorithm>


class Solution {
    public:
        std::string addBinary(std::string a, std::string b) {
            std::string ret = "";
            std::reverse(a.begin(), a.end());
            std::reverse(b.begin(), b.end());
            auto max_len = a.size()>b.size()? a.size(): b.size();
            bool addt = false;
            for(auto i=0; i<max_len; ++i){
                int na = i<a.size()? a[i]-'0': 0;
                int nb = i<b.size()? b[i]-'0': 0;
                int n  = addt? na+nb+1: na+nb;
                switch(n){
                    case 0:
                        ret.append("0"); addt=false; break;
                    case 1:
                        ret.append("1"); addt=false; break;
                    case 2:
                        ret.append("0"); addt=true;  break;
                    case 3:
                        ret.append("1"); addt=true;  break;
                    default:
                        break;
                }
            }
            if(addt) ret.append("1");

            std::reverse(ret.begin(), ret.end());
            return ret;
        }
};


int main(void)
{
    std::cout << Solution().addBinary("0", "0") << std::endl;
    std::cout << Solution().addBinary("0", "1") << std::endl;
    std::cout << Solution().addBinary("1", "1") << std::endl;
    std::cout << Solution().addBinary("10", "10") << std::endl;
    std::cout << Solution().addBinary("11", "1") << std::endl;
    std::cout << Solution().addBinary("11", "11") << std::endl;
    return 0;
}


