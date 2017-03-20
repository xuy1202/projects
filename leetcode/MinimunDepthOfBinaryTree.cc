/*
 * Given a binary tree, find its minimum depth.
 *
 * The minimum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.
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
        int minDepth(TreeNode *root) {
            if(root == NULL){
                return 0;
            }
            if(root->left==NULL && root->right==NULL){
                return 1;
            }
            int ld = minDepth(root->left);
            int rd = minDepth(root->right);
            if(ld == 0) return rd + 1;
            if(rd == 0) return ld + 1;
            return (ld < rd? ld+1: rd+1);
        }
};


int main(void)
{
    TreeNode n1(1);
    TreeNode n2(2);
    TreeNode n3(3);
    TreeNode n4(4);
    TreeNode n5(5);
    
    std::cout << Solution().minDepth(&n1) << std::endl;
    n1.left = &n2;
    std::cout << Solution().minDepth(&n1) << std::endl;
    n1.right = &n3;
    std::cout << Solution().minDepth(&n1) << std::endl;
    n3.right = &n4;
    std::cout << Solution().minDepth(&n1) << std::endl;

    n1.left = &n2;
    n1.right = NULL;
    n2.left = &n3;
    n2.right = NULL;
    n3.left = &n4;
    n3.right = NULL;
    std::cout << Solution().minDepth(&n1) << std::endl;

    n1.left = &n2;
    n1.right = &n3;
    n2.left = &n4;
    n2.right = &n5;
    n3.left = NULL;
    n3.right = NULL;
    std::cout << Solution().minDepth(&n1) << std::endl;

    return 0;
}

