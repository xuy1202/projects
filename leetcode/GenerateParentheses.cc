/*
 * Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.
 *
 * For example, given n = 3, a solution set is:
 *
 * "((()))", "(()())", "(())()", "()(())", "()()()"
 */


#include "helper.h"


class Solution {
    public:
        std::vector<std::string> generateParenthesis(int n) {
            std::vector<std::string> rets;
            if(n <= 0){
                return rets;
            }
            for(size_t i=0; i<n; ++i){
                for(size_t j=0; j<=i; ++j){
                    size_t k = 0;
                    size_t l = 0;
                    while(k++ <= i) std::cout << "(";
                    while(l++ <= j) std::cout << ")"; 
                    //while(k++ <= n) std::cout << "(";
                    //while(l++ <= n) std::cout << ")";
                    std::cout << std::endl;
                }
            }
            return rets;
        }
};


int main(void)
{
    //showVec(Solution().generateParenthesis(0));
    //showVec(Solution().generateParenthesis(1));
    //showVec(Solution().generateParenthesis(2));
    showVec(Solution().generateParenthesis(3));
}

