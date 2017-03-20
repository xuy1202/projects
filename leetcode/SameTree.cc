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
        bool isSameTree(TreeNode *p, TreeNode *q) {
            if(!p && !q){
                return true;
            }
            if((p && ! q) || (q && !p)){
                return false;
            }
            if(p->val != q->val){
                return false;
            }
            return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
        }
};


int main(void)
{
    TreeNode l(0);
    TreeNode r(0);
    TreeNode s(1);
    r.right = s;

    std::cout << Solution().isSameTree(&l, &r) << std::endl;
    return 0;
}
