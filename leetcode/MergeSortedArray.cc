/*
 * Given two sorted integer arrays A and B, merge B into A as one sorted array.
 *
 * Note:
 * You may assume that A has enough space (size that is greater or equal to m + n) to hold additional elements from B. The number of elements initialized in A and B are m and n respectively.
 */

#include <iostream>
#include <vector>


class Solution {
    public:
        // extra mem free
        // otherwise we can use a tmp vector
        void merge(int A[], int m, int B[], int n) {
            int ida = 0;
            int idb = 0;
            int len = m;
            while(true){
                // B is ending
                if(idb >= n){
                    break;
                }
                // A is ending
                if(ida >= len){
                    while(idb < n){
                        A[ida++] = B[idb++];
                    }
                    break;
                }
                int na = A[ida];
                int nb = B[idb];
                // B is bigger
                if(nb >= na){
                    ++ida;
                    continue;
                }
                // A is bigger, Moving A
                int rid = len;
                //std::cout << na << " vs. " << nb << std::endl;
                while(rid > ida){
                    //std::cout << A[rid] << " " << A[rid-1] << std::endl;
                    A[rid] = A[--rid];
                }
                A[ida++] = B[idb++];
                ++len;
            }
        }
};


int main(void)
{
    int A[16] = {1,3,5,7,9};
    int B[16] = {2,4,6,8};
    Solution().merge(A, 5, B, 4);
    for(auto i=0; i<9; ++i){
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;
    int B2[16] = {1,3,5,7,9};
    int A2[16] = {2,4,6,8};
    Solution().merge(A2, 4, B2, 5);
    for(auto i=0; i<9; ++i){
        std::cout << A2[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}


