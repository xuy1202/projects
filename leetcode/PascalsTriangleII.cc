/*
 * Given an index k, return the kth row of the Pascal's triangle.
 *
 * For example, given k = 3,
 * Return [1,3,3,1].
 */

#include<iostream>
#include<vector>



void show_vec(const std::vector<int>& out)
{
    std::cout << "[";
    for(auto i=out.begin(); i!= out.end(); ++i){
        if(i!=out.begin()){
            std::cout << ", ";
        }
        std::cout << *i;
    }
    std::cout << "]" << std::endl;
}


class Solution {
    public:
        std::vector<int> getRow(int rowIndex) {
            static struct _gen_next{
                void operator() (const std::vector<int>& in, std::vector<int>& out) const {
                    if(in.size() == 0){
                        out.push_back(1);
                        return;
                    }
                    if(in.size() == 1){
                        out.push_back(1);
                        out.push_back(1);
                        return;
                    }
                    out.push_back(1);
                    for(auto i=0; i<in.size()-1; ++i){
                        out.push_back(in[i] + in[i+1]);
                    }
                    out.push_back(1);
                }
            } gen_next;

            std::vector<int> sed;
            std::vector<int> nxt;
            if(rowIndex < 0) return sed; 

            for(auto i=0; i<=rowIndex; ++i){
                nxt.clear();
                gen_next(sed, nxt);
                sed.swap(nxt);
            }
            return sed;
        }
};


int main(void)
{
    show_vec(Solution().getRow(-1));
    show_vec(Solution().getRow(0));
    show_vec(Solution().getRow(1));
    show_vec(Solution().getRow(2));
    show_vec(Solution().getRow(3));
    show_vec(Solution().getRow(4));
    return 0;
}


