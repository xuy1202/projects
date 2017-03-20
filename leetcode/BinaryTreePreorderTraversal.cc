/*
 * Given a binary tree, return the preorder traversal of its nodes' values.
 *
 * For example:
 * Given binary tree {1,#,2,3},
 *    1
 *     \
 *      2
 *     /
 *    3
 *  return [1,2,3].
 *
 *  Note: Recursive solution is trivial, could you do it iteratively?
 */

#include "helper.h"


class Solution1 {
    public:
        void preorderTraversal(TreeNode* root, std::vector<int>& ret)
        {
            if(root == NULL){
                return;
            }
            ret.push_back(root->val);
            preorderTraversal(root->left, ret);
            preorderTraversal(root->right, ret);
            return;
        }

        std::vector<int> preorderTraversal(TreeNode *root)
        {
            std::vector<int> ret;
            preorderTraversal(root, ret);
            return ret;
        }
};


class Solution {
    public:
        std::vector<int> preorderTraversal(TreeNode *root)
        {
            std::stack<TreeNode*>  stk;
            std::vector<int>       ret;
            stk.push(root);
            do{
                if(stk.empty()){
                    break;
                }
                root = stk.top(); 
                stk.pop();
                if(root == NULL){
                    continue;
                }
                ret.push_back(root->val);
                stk.push(root->right);
                stk.push(root->left);
            }while(true);

            return ret;
        }
};


int main(void)
{
    TreeNode root1(1);
    TreeNode root2(2);
    TreeNode root3(3);
    root1.right = &root2;
    root2.left  = &root3;
    showVec(Solution().preorderTraversal(&root1));

    return 0;
}

