/*
 * Given an array and a value, remove all instances of that value in place and return the new length.
 *
 * The order of elements can be changed. It doesn't matter what you leave beyond the new length. 
 */

#include <iostream>


void showElement(int A[], int n)
{
    std::cout << "\t" << n << ": ";
    for(auto i=0; i<n; ++i){
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;
}


class Solution {
    public:
        int removeElement(int A[], int n, int elem) {
            if(n == 0){
                return 0;
            }
            int l = n-1;
            int i = 0;
            while(i < l){
                if(A[i] != elem){
                    ++i;
                }else{
                    A[i] = A[l--];
                }
            }
            return (A[i]==elem? l: l+1);
        }
};



int main(void)
{
    int a[5] = {1,1,1,4,5};
    showElement(a, Solution().removeElement(a, 5, 1));

    int b[5] = {1,2,3,4,5};
    showElement(b, Solution().removeElement(b, 5, 1));

    int c[5] = {1,2,3,1,1};
    showElement(c, Solution().removeElement(c, 5, 1));

    int d[5] = {1,1,1,1,1};
    showElement(d, Solution().removeElement(d, 5, 1));

    return 0;
}



