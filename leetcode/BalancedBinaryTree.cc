/*
 * Given a binary tree, determine if it is height-balanced.
 *
 * For this problem, a height-balanced binary tree is defined as a binary tree in which the depth of the two subtrees of every node never differ by more than 1. 
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
        int maxBalancedDepth(TreeNode* root){
            if(! root) return 0;
            if(! root->left && ! root->right) return 1;
            if(root->left && root->right){
                int ld = maxBalancedDepth(root->left);
                if(ld == -1) return -1;
                int rd = maxBalancedDepth(root->right);
                if(rd == -1) return -1;
                if(abs(rd-ld)> 1) return -1;
                return (ld>=rd? ld+1: rd+1);
            }
            if(root->left){
                if(root->left->left || root->left->right) return -1;
            }
            if(root->right){
                if(root->right->left || root->right->right) return -1;
            }
            return 2;
        }

        bool isBalanced(TreeNode *root) {
            if(root == NULL){
                return true;
            }
            int mbl = maxBalancedDepth(root->left);
            if(mbl == -1){
                return false;
            }
            int mbr = maxBalancedDepth(root->right);
            if(mbr == -1){
                return false;
            }
            return (abs(mbl-mbr) <= 1); 
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
    
    std::cout << Solution().isBalanced(&n1) << std::endl;
    n1.left = &n2;
    std::cout << Solution().isBalanced(&n1) << std::endl;
    n2.left = &n4;
    std::cout << Solution().isBalanced(&n1) << std::endl;
    n1.right = &n3;
    std::cout << Solution().isBalanced(&n1) << std::endl;

    n2.right = &n5;
    n4.left = &n6;
    std::cout << Solution().isBalanced(&n1) << std::endl;
    n4.right = &n7;
    std::cout << Solution().isBalanced(&n1) << std::endl;

    return 0;
}

