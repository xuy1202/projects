/*
 * The gray code is a binary numeral system where two successive values differ in only one bit.
 *
 * Given a non-negative integer n representing the total number of bits in the code, print the sequence of gray code. A gray code sequence must begin with 0.
 *
 * For example, given n = 2, return [0,1,3,2]. Its gray code sequence is:
 *
 * 00 - 0
 * 01 - 1
 * 11 - 3
 * 10 - 2
 * Note:
 * For a given n, a gray code sequence is not uniquely defined.
 *
 * For example, [0,2,3,1] is also a valid gray code sequence according to the above definition.
 *
 * For now, the judge is able to judge based on one instance of gray code sequence. Sorry about that.
 */

#include "helper.h"


class Solution {
    public:
        std::vector<int> grayCode(int n) {
            std::vector<int> rets = {0};
            int i = 0;
            while(i < n){
                int add = 1 << i++;
                int idx = rets.size() - 1;
                while(idx >= 0 ){
                    rets.push_back(rets[idx--] + add);
                }
            }
            return rets;
        }
};


int main(void)
{
    showVec(Solution().grayCode(0));
    showVec(Solution().grayCode(1));
    showVec(Solution().grayCode(2));
    showVec(Solution().grayCode(3));
}



