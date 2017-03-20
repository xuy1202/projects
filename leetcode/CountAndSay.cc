//The count-and-say sequence is the sequence of integers beginning as follows:
//1, 11, 21, 1211, 111221, ...
//
//1 is read off as "one 1" or 11.
//11 is read off as "two 1s" or 21.
//21 is read off as "one 2, then one 1" or 1211.
//Given an integer n, generate the nth sequence.

#include <iostream>
#include <vector>
#include <string.h>


class Solution {
    public:
        std::string countAndSay(int n)
        {
            if(n <= 0) return "";

            std::string s = "1";
            for(auto i=1; i<n; ++i){
                s = _countAndSay(s);
            }
            return s;
        }

        std::string _countAndSay(const std::string& n) {
            struct Item{
                int num;
                int cnt;

                Item(int n, int c): num(n), cnt(c){
                }
            };

            std::vector<Item> l;
            for(auto i=0; i<n.size(); ++i){
                int j = n[i] - '0';
                if(l.empty() or l.rbegin()->num != j){
                    l.push_back(Item(j, 1));
                }else{
                    ++(l.rbegin()->cnt);
                }
            }

            char buf[10240] = {0};
            for(auto i=0; i<l.size(); ++i){
                const Item& item = l[i];
                buf[2*i] = item.cnt + '0';
                buf[2*i + 1] = item.num + '0';
            }
            buf[l.size() * 2] = 0x0;
            return std::string(buf);
        }
};


int main(void)
{
    std::cout << Solution().countAndSay(0) << std::endl;
    std::cout << Solution().countAndSay(1) << std::endl;
    std::cout << Solution().countAndSay(2) << std::endl;
    std::cout << Solution().countAndSay(3) << std::endl;
    std::cout << Solution().countAndSay(4) << std::endl;
    std::cout << Solution().countAndSay(5) << std::endl;
    std::cout << Solution().countAndSay(6) << std::endl;
    std::cout << Solution().countAndSay(7) << std::endl;
    std::cout << Solution().countAndSay(8) << std::endl;
    std::cout << Solution().countAndSay(9) << std::endl;
    std::cout << Solution().countAndSay(10) << std::endl;
    std::cout << Solution().countAndSay(11) << std::endl;
    std::cout << Solution().countAndSay(12) << std::endl;
    std::cout << Solution().countAndSay(13) << std::endl;
    std::cout << Solution().countAndSay(14) << std::endl;
    std::cout << Solution().countAndSay(15) << std::endl;
    return 0;
}

