/*
 *  Given a sorted array, remove the duplicates in place such that each element appear only once and return the new length.
 *
 *  Do not allocate extra space for another array, you must do this in place with constant memory.
 *
 *  For example,
 *  Given input array A = [1,1,2],
 *
 *  Your function should return length = 2, and A is now [1,2]. 
 */

#include <iostream>
#include <set>


void showElement(int A[], int n)
{
    std::cout << "\t" << n << ": ";
    for(auto i=0; i<n; ++i){
        std::cout << A[i] << " ";
    }
    std::cout << std::endl;
}


class Solution1 {
    public:
        inline void moveForword(int A[], int f, int l)
        {
            int i = 0;
            for(; i<l-f; ++i){
                A[i] = A[f+i];
            }
        }

        int removeDuplicates(int A[], int n)
        {
            if(n == 0){
                return 0;
            }
            int l = n-1;
            int i = 1;
            while(i <= l){
                int c = A[i-1];
                if(A[i] != c){
                    ++i;
                    continue;
                }
                moveForword(A+i, 1, (--l)-i+2);
            }
            return l+1; 
        }
};


class Solution {
    public:
        int removeDuplicates(int A[], int n)
        {
            std::set<int> set;
            for(auto i=0; i<n; ++i){
                set.insert(A[i]);
            }
            int i = 0;
            for(auto it=set.begin(); it!=set.end(); ++it){
                A[i++] = *it;
            }
            return set.size();
        }
};

int main(void)
{
    int a[10] = {1,1,2,2,2,3,4,5,5,6};
    showElement(a, Solution().removeDuplicates(a, 10));

    int b[5] = {1,1,1,1,1};
    showElement(b, Solution().removeDuplicates(b, 5));

    int c[1] = {1};
    showElement(c, Solution().removeDuplicates(c, 1));

    return 0;
}


