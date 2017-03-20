/*
 * Given a set of distinct integers, S, return all possible subsets.
 *
 * Note:
 * Elements in a subset must be in non-descending order.
 * The solution set must not contain duplicate subsets.
 * For example,
 * If S = [1,2,3], a solution is:
 *
 * [
 *   [3],
 *   [1],
 *   [2],
 *   [1,2,3],
 *   [1,3],
 *   [2,3],
 *   [1,2],
 *   []
 * ]
 */

#include "helper.h"


class Solution {
    public:
        std::vector< std::vector<int> > subsets(std::vector<int> &S)
        {
            struct _GenNextIds{
                bool operator () (std::vector<int>& idvec, int max=4)
                {
                    bool rotated = false;
                    for(int i=idvec.size()-1; i>=0; --i){
                        if(idvec[i] < max - idvec.size() + i){
                            idvec[i] += 1;
                            if(rotated && idvec[i] == idvec[i+1]){
                                i = idvec.size();
                                rotated = false;
                                continue;
                            }
                            return true;
                        }
                        if(i > 0){
                            idvec[i] = idvec[i-1] + 1;
                            rotated = true;
                        }
                    }
                    int i = 0;
                    while(i < idvec.size()){
                        idvec[i] = i++;
                    }
                    idvec.push_back(idvec.size());
                    return (idvec.size() <= max);
                }
            } GenNextIds;

            std::sort(S.begin(), S.end());
            std::vector<int> idvec;
            std::vector< std::vector<int> > ret;
            // always have an empty set
            ret.push_back(std::vector<int>());
            while(GenNextIds(idvec, S.size())){
                //showVec(idvec);
                ret.push_back(std::vector<int>());
                for(auto it=idvec.begin(); it!=idvec.end(); ++it){
                    ret.rbegin()->push_back(S[*it]);
                }
            }
            return ret;
        }
};


int main(void)
{
    std::vector< std::vector<int> > ret;

    std::vector<int> s3 = {1,2,3};
    ret = Solution().subsets(s3);
    for(auto it=ret.begin(); it!=ret.end(); ++it){
        showVec(*it);
    }

    std::cout << "===========" << std::endl;

    std::vector<int> s4 = {4,1,2,3};
    ret = Solution().subsets(s4);
    for(auto it=ret.begin(); it!=ret.end(); ++it){
        showVec(*it);
    }

    std::cout << "===========" << std::endl;

    std::vector<int> s6 = {6, 5, 4,1,2,3};
    ret = Solution().subsets(s6);
    for(auto it=ret.begin(); it!=ret.end(); ++it){
        showVec(*it);
    }

    return 0;
}



