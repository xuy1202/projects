/*
 * Suppose a sorted array is rotated at some pivot unknown to you beforehand.
 *
 * (i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).
 *
 * Find the minimum element.
 *
 * You may assume no duplicate exists in the array.
 */


#include "helper.h"


class Solution {
    public:
        int findMin(std::vector<int> &num) {
            int beg = 0;
            int end = num.size() - 1;
            int mid = -1;
            while(beg <= end){
                mid = beg + ((end-beg) >> 1);
                // beg = mid = end - 1
                if(mid == beg) break;
                if(num[mid] > num[end]){
                    beg = mid;
                }else{
                    end = mid;
                }
                // assume no duplicate, so no equeal
            }
            return std::min(num[beg], num[end]);
        }
};


int main(void)
{
    std::vector<int> num1 = {4,5,6,7,0,1,2};
    std::cout << Solution().findMin(num1) << std::endl;

    std::vector<int> num2 = {2,4,5,6,7,0,1};
    std::cout << Solution().findMin(num2) << std::endl;

    std::vector<int> num3 = {1,2,4,5,6,7,0};
    std::cout << Solution().findMin(num3) << std::endl;

    std::vector<int> num4 = {5,6,7,0,1,2,4};
    std::cout << Solution().findMin(num4) << std::endl;

    return 0;
}


