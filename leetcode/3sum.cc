/*
 * Given an array S of n integers, are there elements a, b, c in S such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.
 *
 * Note:
 * Elements in a triplet (a,b,c) must be in non-descending order. (ie, a ≤ b ≤ c)
 * The solution set must not contain duplicate triplets.
 *
 *     For example, given array S = {-1 0 1 2 -1 -4},
 *
 *     A solution set is:
 *     (-1, 0, 1)
 *     (-1, -1, 2)
 */

#include "helper.h"


class Solution1 {
    public:
        struct GenNextIds{
            bool operator () (std::vector<int>& v, int max=4)
            {
                bool rotd = false;
                auto sz = v.size();
                for(int i=sz-1; i>=0; --i){
                    if(v[i] < max - sz + i){
                        v[i] += 1;
                        if(! rotd || v[i] != v[i+1]){
                            return true;
                        }
                        i = sz;
                        rotd = false;
                        continue;
                    }
                    v[i] = v[i-1] + 1;
                    rotd = true;
                }
                return false;
            }
        };


        std::vector<std::vector<int> > threeSum(std::vector<int>& num)
        {
            /*
             * dedupe
             */
            //std::sort(num.begin(), num.end());
            //num.erase(std::unique(num.begin(), num.end()), num.end());
            std::vector< std::vector<int> > ret;
            GenNextIds                 gen_next;

            auto size = num.size();
            if(size < 3){
                return ret;
            }

            std::vector<int> idvec = {0, 1, 2};
            do{
                int a = num[idvec[0]];
                int b = num[idvec[1]];
                int c = num[idvec[2]];
                if(a + b + c != 0){
                    continue;
                }
                std::vector<int> vec = {a, b, c};
                std::sort(vec.begin(), vec.end());
                ret.push_back(vec);
            }while(gen_next(idvec, size));

            std::sort(ret.begin(), ret.end());
            ret.erase(std::unique(ret.begin(), ret.end()), ret.end());

            return ret;
        }
};


class Solution {
    public:
        std::vector<std::vector<int> > threeSum(std::vector<int>& num)
        {
            std::vector< std::vector<int> > ret;
            if(num.size() < 3){
                return ret;
            }
            std::sort(num.begin(), num.end());
            for(uint32_t i=0; i<=num.size()-3; ++i){
                if(i!=0 && num[i] == num[i-1]){
                    continue;
                }
                int a = num[i];
                for(auto j=i+1; j<=num.size()-2; ++j){
                    if(j!=i+1 && num[j] == num[j-1]){
                        continue;
                    }
                    int b = num[j];
                    int c = 0-a-b;
                    if(std::binary_search(num.begin()+j+1, num.end(), c)){
                        ret.push_back({a, b, c});
                    }
                }
            }
            return ret;
        }
};


int main(void)
{
    //std::vector<int> ini = {-1, 0, 1, 2, -1, 4};
    //std::vector<int> ini = {0, 0, 0};
    std::vector<int> ini = {7,5,-8,-6,-13,7,10,1,1,-4,-14,0,-1,-10,1,-13,-4,6,-11,8,-6,0,0,-5,0,11,-9,8,2,-6,4,-14,6,4,-5,0,-12,12,-13,5,-6,10,-10,0,7,-2,-5,-12,12,-9,12,-9,6,-11,1,14,8,-1,7,-13,8,-11,-11,0,0,-1,-15,3,-11,9,-7,-10,4,-2,5,-4,12,7,-8,9,14,-11,7,5,-15,-15,-4,0,0,-11,3,-15,-15,7,0,0,13,-7,-12,9,9,-3,14,-1,2,5,2,-9,-3,1,7,-12,-3,-1,1,-2,0,12,5,7,8,-7,7,8,7,-15};

    std::vector<std::vector<int> > ret = Solution().threeSum(ini);
    for(auto it=ret.begin(); it!=ret.end(); ++it){
        showVec(*it);
    }
    return 0;
}



