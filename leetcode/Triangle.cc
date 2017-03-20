/*
 * Given a triangle, find the minimum path sum from top to bottom. Each step you may move to adjacent numbers on the row below.
 *
 * For example, given the following triangle
 * [
 *      [2],
 *     [3,4],
 *    [6,5,7],
 *    [4,1,8,3]
 * ]
 * The minimum path sum from top to bottom is 11 (i.e., 2 + 3 + 5 + 1 = 11).
 *
 * Note:
 *   Bonus point if you are able to do this using only O(n) extra space, where n is the total number of rows in the triangle.
 */


#include "helper.h"


class Solution {
    public:
        int minimumTotal(std::vector<std::vector<int> > &triangle) {
            int ret = 0;
            for(auto it=triangle.begin(); it!=triangle.end(); ++it){
                std::sort(it->begin(), it->end());
                ret += *(it->begin());
            }
            return ret;
        }
};


int main(void)
{
    std::vector<std::vector<int> > t;
    t.push_back({2});
    t.push_back({3,4});
    t.push_back({6,5,7});
    t.push_back({4,1,8,3});
    std::cout << Solution().minimumTotal(t) << std::endl;
    return 0;
}


