/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
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

        bool isSymmetric(TreeNode *root)
        {
            static struct _isReflect{
                bool operator()(TreeNode* lh, TreeNode*rh) const
                {
                    if(!lh && !rh) return true;
                    if((lh && !rh) || (rh && !lh)) return false;
                    if(lh->val != rh->val) return false;
                    return (*this)(lh->left, rh->right) && (*this)(lh->right, rh->left);
                }
            } isReflect;

            if(! root) return true;
            return isReflect(root->left, root->right);
        }
};


int main(void)
{
    TreeNode root(0);
    TreeNode l(1);
    TreeNode r(1);
    root.left = &l;
    //root.right = &r;

    std::cout << Solution().isSymmetric(&root) << std::endl;
    return 0;
}
