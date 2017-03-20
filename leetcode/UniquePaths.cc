/*
 * A robot is located at the top-left corner of a m x n grid (marked 'Start' in the diagram below).
 *
 * The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).
 *
 * How many possible unique paths are there?
 */


#include "helper.h"


class Solution {
    private:
        int rets[101][101];

    public:
        Solution()
        {
            memset(rets, 0, sizeof(rets));
        }

    public:
        int uniquePaths(int m, int n) {
            if(m <= 0 && n <= 0){
                return 0;
            }
            if(m == 1){
                return n>=1? 1: 0;
            }
            if(n == 1){
                return m>=1? 1: 0;
            }
            int r = 0;
            r = (m > n)? rets[n][m]: rets[m][n];
            if(r == 0){
                r = uniquePaths(m-1, n) + uniquePaths(m, n-1);
                if(m > n){
                    rets[n][m] = r;
                }else{
                    rets[m][n] = r;
                }
            }
            return r;
        }
};


int main(void)
{
    std::cout << Solution().uniquePaths(0, 0) << std::endl;
    std::cout << Solution().uniquePaths(1, 0) << std::endl;
    std::cout << Solution().uniquePaths(0, 1) << std::endl;
    std::cout << Solution().uniquePaths(1, 1) << std::endl;

    std::cout << Solution().uniquePaths(1, 2) << std::endl;
    std::cout << Solution().uniquePaths(2, 1) << std::endl;

    std::cout << Solution().uniquePaths(2, 2) << std::endl;

    std::cout << Solution().uniquePaths(2, 3) << std::endl;

    std::cout << Solution().uniquePaths(3, 3) << std::endl;

    std::cout << Solution().uniquePaths(2, 4) << std::endl;

    std::cout << Solution().uniquePaths(3, 7) << std::endl;
}

