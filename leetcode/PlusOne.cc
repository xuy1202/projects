/*
 * Given a non-negative number represented as an array of digits, plus one to the number.
 *
 * The digits are stored such that the most significant digit is at the head of the list.
 */

#include <iostream>
#include <vector>
#include <algorithm>


class Solution {
    public:
        std::vector<int> plusOne(std::vector<int> &digits) {
            std::vector<int> rev;
            if(digits.empty()){
                rev.push_back(1);
                return rev;
            }
            rev.push_back((*digits.rbegin()) + 1);
            for(auto it=digits.rbegin()+1; it!=digits.rend(); ++it){
                rev.push_back(*it);
            }
            for(auto it=rev.begin(); it!=rev.end(); ++it){
                if(*it <= 9) continue;
                *it = (*it) % 10;
                if(it+1 == rev.end()){
                    rev.push_back(1);
                    break;
                }else{
                    *(it+1) = *(it+1) + 1;
                }
            }
            std::reverse(rev.begin(), rev.end());
            return rev;
        }
};

void showVec(const std::vector<int> v)
{
    for(auto it=v.begin(); it!=v.end(); ++it){
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}


int main(void)
{
    std::vector<int> v;
    showVec(Solution().plusOne(v));
    v.push_back(2);
    showVec(Solution().plusOne(v));
    v.push_back(9);
    showVec(Solution().plusOne(v));
    v.push_back(9);
    showVec(Solution().plusOne(v));
    return 0;
}



