/*
 * Given an array of size n, find the majority element. The majority element is the element that appears more than ? n/2 ? times.
 *
 * You may assume that the array is non-empty and the majority element always exist in the array.
 */

#include "helper.h"


class Solution {
    public:
        int majorityElement(std::vector<int> &num) {
            std::map<int, uint32_t> m;
            for(auto it=num.begin(); it!=num.end(); ++it){
                m[*it] += 1; 
            }
            uint32_t c = 0;
            int      n = 0;
            for(auto it=m.begin(); it!=m.end(); ++it){
                if(it->second > c){
                    n = it->first;
                    c = it->second;
                }
            }
            return n;
        }
};


int main(void)
{
    std::vector<int> l = {1,2,3,4,1,1,1};
    std::cout << Solution().majorityElement(l) << std::endl;

    return 0;
}


