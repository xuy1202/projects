/*
 * Given a binary tree, return the bottom-up level order traversal of its nodes' values. (ie, from left to right, level by level from leaf to root).
 *
 * For example:
 * Given binary tree {3,9,20,#,#,15,7},
 *
 *     3
 *    / \
 *   9  20
 *  /  \
 * 15   7
 *
 * return its bottom-up level order traversal as:
 * [
 *   [15,7],
 *   [9,20],
 *   [3]
 * ]
 */


#include <iostream>
#include <vector>
#include <algorithm>


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};


class Solution {
    public:
        void walkCollect(TreeNode* root, int layer, std::vector<std::vector<int> >& vec)
        {
            if(root == NULL) return;
            while(vec.size() <= layer){
                vec.push_back(std::vector<int>());
            }
            vec[layer].push_back(root->val);
            walkCollect(root->left,  layer+1, vec);
            walkCollect(root->right, layer+1, vec);
        }

        std::vector<std::vector<int> > levelOrderBottom(TreeNode *root)
        {
            std::vector<std::vector<int> > retvec;
            walkCollect(root, 0, retvec);
            std::reverse(retvec.begin(), retvec.end());
            return retvec;
        }
};


int main(void)
{
    TreeNode n1(1);
    TreeNode n2(2);
    TreeNode n3(3);
    TreeNode n4(4);
    TreeNode n5(5);
    TreeNode n6(6);
    TreeNode n7(7);
    
    n1.left = &n2;
    n2.left = &n4;
    n1.right = &n3;
    n2.right = &n5;
    n4.left = &n6;
    n3.right = &n7;

    auto vv = Solution().levelOrderBottom(&n1);
    for(auto it=vv.begin(); it!=vv.end(); ++it){
        std::cout << "[";
        for(auto jt=it->begin(); jt!=it->end(); ++jt){
            std::cout << *jt << " ";
        }
        std::cout << "]" << std::endl;
    }

    n2.left = n2.right = NULL;
    n3.left = &n6;
    n6.left = n6.right = NULL;
    vv = Solution().levelOrderBottom(&n1);
    for(auto it=vv.begin(); it!=vv.end(); ++it){
        std::cout << "[";
        for(auto jt=it->begin(); jt!=it->end(); ++jt){
            std::cout << *jt << " ";
        }
        std::cout << "]" << std::endl;
    }
    return 0;
}


