/*
 * Write a function to find the longest common prefix string amongst an array of strings.
 */

#include <iostream>
#include <vector>
#include <string.h>


class Solution {
    public:
        std::string longestCommonPrefix(const std::vector<std::string>& strs) {
            if(strs.empty())     return "";
            if(strs.size() == 1) return strs[0];
            
            std::string ret = "";
            size_t len = strs.max_size();
            for(auto it=strs.begin(); it!=strs.end(); ++it){
                len = (it->size() < len) ? it->size() : len; 
            }
            for(auto idx=0; idx<len; ++idx){
                for(auto it=strs.begin()+1; it!=strs.end(); ++it){
                    if(strs[0][idx] != (*it)[idx]) return ret;
                }
                ret.append(std::string(1, strs[0][idx]));
            }

            return ret;
        }
};

int main(void)
{
    std::vector<std::string>  v1 = {};
    std::cout << Solution().longestCommonPrefix(v1) << std::endl;

    std::vector<std::string>  v2 = {""};
    std::cout << Solution().longestCommonPrefix(v2) << std::endl;

    std::vector<std::string>  v3 = {"aa"};
    std::cout << Solution().longestCommonPrefix(v3) << std::endl;

    std::vector<std::string>  v4 = {"aa", "ab"};
    std::cout << Solution().longestCommonPrefix(v4) << std::endl;

    std::vector<std::string>  v5 = {"aab", "aac"};
    std::cout << Solution().longestCommonPrefix(v5) << std::endl;

    std::vector<std::string>  v6 = {"aab", "aac", ""};
    std::cout << Solution().longestCommonPrefix(v6) << std::endl;

    return 0;
}
