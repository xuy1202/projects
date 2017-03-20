/*
 * Given a binary tree, find its maximum depth.
 *
 * The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.
 */

#include <iostream>


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};


class Solution {
    public:
        int maxDepth(TreeNode *root) {
            if(root == NULL){
                return 0;
            }
            
            int ld = 1 + maxDepth(root->left);
            int rd = 1 + maxDepth(root->right);
            
            return (ld>=rd? ld: rd);
        }
};


int main(void)
{
    TreeNode n1(1);
    TreeNode n2(2);
    TreeNode n3(3);
    TreeNode n4(4);
    
    n1.left = &n2;
    n1.right = &n3;
    n3.right = &n4;

    std::cout << Solution().maxDepth(&n1) << std::endl;
    return 0;
}

