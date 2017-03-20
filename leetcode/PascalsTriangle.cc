/*
 * Given numRows, generate the first numRows of Pascal's triangle.
 *
 * For example, given numRows = 5,
 * Return
 *
 * [
 *      [1],
 *     [1,1],
 *    [1,2,1],
 *   [1,3,3,1],
 *  [1,4,6,4,1]
 * ]
 *
 *
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
		std::vector< std::vector<int> > generate(int numRows) {
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

			std::vector< std::vector<int> > ret;
			if(numRows < 1){
				return ret;
			}
			std::vector<int> inv;
			gen_next(inv, inv);
			ret.push_back(inv);
			for(auto i=2; i<=numRows; ++i){
				inv.clear();
				gen_next(*(ret.rbegin()), inv);
				ret.push_back(inv);
			}
			return ret;
		}
};


int main(void)
{
	Solution().generate(0);
	Solution().generate(1);
	Solution().generate(2);
	Solution().generate(5);
	return 0;
}

