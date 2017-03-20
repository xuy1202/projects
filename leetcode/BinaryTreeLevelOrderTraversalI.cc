/*
 * Given a binary tree, return the level order traversal of its nodes' values. (ie, from left to right, level by level).
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
 * return its level order traversal as:
 * [
 *   [3],
 *   [9,20],
 *   [15,7]
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

        std::vector<std::vector<int> > levelOrder(TreeNode *root)
        {
            std::vector<std::vector<int> > retvec;
            walkCollect(root, 0, retvec);
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

    auto vv = Solution().levelOrder(&n1);
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
    vv = Solution().levelOrder(&n1);
    for(auto it=vv.begin(); it!=vv.end(); ++it){
        std::cout << "[";
        for(auto jt=it->begin(); jt!=it->end(); ++jt){
            std::cout << *jt << " ";
        }
        std::cout << "]" << std::endl;
    }
    return 0;
}


