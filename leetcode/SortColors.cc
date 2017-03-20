/*
 * Given an array with n objects colored red, white or blue, sort them so that objects of the same color are adjacent, with the colors in the order red, white and blue.
 *
 * Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue respectively.
 *
 * Note:
 * You are not suppose to use the library's sort function for this problem.
 *
 * click to show follow up.
 *
 * Follow up:
 * A rather straight forward solution is a two-pass algorithm using counting sort.
 * First, iterate the array counting number of 0's, 1's, and 2's, then overwrite array with total number of 0's, then 1's and followed by 2's.
 *
 * Could you come up with an one-pass algorithm using only constant space?
 */


#include "helper.h"


class Solution {
    private:
        void swap(int* lh, int* rh)
        {
            *lh = (*lh) ^ (*rh);
            *rh = (*lh) ^ (*rh);
            *lh = (*lh) ^ (*rh);
        }

    public:
        void sortColors(int A[], int n) {
            int idx0 = 0;
            int idx2 = n-1;
            int indx = 0;
            while(true){
                if(indx >  idx2) break;
                if(indx <  idx0) break;
                if(idx0 >= n   ) break;
                if(idx2 <  0   ) break;

                if(A[indx] == 0 && idx0 != indx){
                    swap(A+idx0++, A+indx);
                    continue;
                }
                if(A[indx] == 2 && idx2 != indx){
                    swap(A+idx2--, A+indx);
                    continue;
                }
                ++indx;
            }
        }
};


int main(void)
{
    //int A[9] = {0,1,2,1,2,0,1,1,2};
    int A[2] = {2, 1};
    //showArr(A, 2);
    Solution().sortColors(A, 2);
    //showArr(A, 2);

    return 0;
}


