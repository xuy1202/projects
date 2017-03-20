/*
 * Given an array of integers, every element appears twice except for one. Find that single one.
 *
 * Note:
 * Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?
 */

#include "helper.h"


class Solution {
    public:
        int singleNumber(int A[], int n) {
            int number = 0;
            for(auto i=0; i<n; ++i){
                number ^= A[i];
            }
            return number;
        }
};


int main(void)
{
    int buf1[] = {1,1,2};
    std::cout << Solution().singleNumber(buf1, 3) << std::endl;

    int buf2[] = {1,1,2,3,3};
    std::cout << Solution().singleNumber(buf2, 5) << std::endl;

    return 0;
}



