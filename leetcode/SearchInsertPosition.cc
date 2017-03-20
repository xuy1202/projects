/*
 * Given a sorted array and a target value, return the index if the target is found. If not, return the index where it would be if it were inserted in order.
 *
 * You may assume no duplicates in the array.
 *
 * Here are few examples.
 * [1,3,5,6], 5 → 2
 * [1,3,5,6], 2 → 1
 * [1,3,5,6], 7 → 4
 * [1,3,5,6], 0 → 0
 */

#include "helper.h"


class Solution {
    public:
        int searchInsert(int A[], int n, int target)
        {
            unsigned beg = 0;
            unsigned end = n-1;
            unsigned mid = 0;
            while(beg <= end && end < n && beg < n){
                mid = (end - beg)/2 + beg;
                if(target < A[mid]){
                    end = mid - 1;
                    continue;
                }
                if(target > A[mid]){
                    beg = mid + 1;
                    continue;
                }
                break;
            }
            // open for bsearch
            //return (target == A[mid]? mid: -1);
            return (target <= A[mid]? mid: mid+1);
        }
};


int main(void)
{
    int l[] = {1,3,5,6};
    std::cout << Solution().searchInsert(l, 4, 5) << std::endl;
    std::cout << Solution().searchInsert(l, 4, 2) << std::endl;
    std::cout << Solution().searchInsert(l, 4, 7) << std::endl;
    std::cout << Solution().searchInsert(l, 4, 0) << std::endl;
    return 0;
}
