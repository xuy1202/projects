/*
 * Given a binary tree and a sum, determine if the tree has a root-to-leaf path such that adding up all the values along the path equals the given sum.
 *
 * For example:
 * G
 *               5
 *              / \
 *             4   8
 *            /   / \
 *           11  13  4
 *          /  \      \
 *         7    2      1
 * return true, as there exist a root-to-leaf path 5->4->11->2 which sum is 22.
 */

#include<iostream>


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};


class Solution {
    public:
        bool hasPathSum(TreeNode *root, int sum) {
            if(! root){
                // return (sum == 0);
                return false;
            }

            if(root->left == NULL && root->right == NULL){
                return (sum == root->val);
            }

            bool ret = false;
            ret = ret || (root->left ? hasPathSum(root->left , sum - root->val): false);
            ret = ret || (root->right? hasPathSum(root->right, sum - root->val): false);
            return ret;
        }
};

int main(void)
{
    TreeNode   n5(5);
    TreeNode n4_1(4);
    TreeNode   n8(8);
    TreeNode  n11(11);
    TreeNode  n13(13);
    TreeNode n4_2(4);
    TreeNode   n7(7);
    TreeNode   n2(2);
    TreeNode   n1(1);
    
    n5.left  = &n4_1;
    n5.right = &n8;

    n4_1.left  = &n11;

    n11.left  = &n7;
    n11.right = &n2;
    
    n8.left  = &n13;
    n8.right = &n4_2;

    n4_2.right = &n1;

    std::cout << Solution().hasPathSum(&n5, 22) << std::endl;
    return 0;
}



