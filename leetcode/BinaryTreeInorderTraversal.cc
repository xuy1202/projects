/*
 * Given a binary tree, return the inorder traversal of its nodes' values.
 *
 * For example:
 * Given binary tree {1,#,2,3},
 *    1
 *     \
 *      2
 *     /
 *    3
 * return [1,3,2].
 *
 * Note: Recursive solution is trivial, could you do it iteratively?
 *
 *     confused what "{1,#,2,3}" means? > read more on how binary tree is serialized on OJ.
 *
 *     OJ's Binary Tree Serialization:
 *     The serialization of a binary tree follows a level order traversal, where '#' signifies a path terminator where no node exists below.
 *
 *     Here's an example:
 *         1
 *        / \
 *       2   3
 *          /
 *         4
 *          \
 *           5
 *   The above binary tree is serialized as "{1,2,3,#,#,4,#,#,5}".
 */


#include "helper.h"

class Solution {
    public:
        void inorderTraversal(TreeNode* root, std::vector<int>& ret)
        {
            if(root == NULL){
                return;
            }
            inorderTraversal(root->left, ret);
            ret.push_back(root->val);
            inorderTraversal(root->right, ret);
            return;
        }

        std::vector<int> inorderTraversal(TreeNode *root)
        {
            std::vector<int> ret;
            inorderTraversal(root, ret);
            return ret;
        }
};

class Solution2 {
    public:
        std::vector<int> inorderTraversal(TreeNode *root)
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
                if(! root->left && ! root->right){
                    ret.push_back(root->val);
                    continue;
                }
                if(! root->left &&   root->right){
                    ret.push_back(root->val);
                    stk.push(root->right);
                    continue;
                }
                if(  root->left && ! root->right){
                    stk.push(root);
                    stk.push(root->left);
                    root->left = NULL;
                    continue;
                }

                stk.push(root->right);
                stk.push(root);
                stk.push(root->left);
                root->left  = NULL;
                root->right = NULL;
            }while(true);

            return ret;
        }
};


int main(void)
{
    TreeNode root1(1);
    TreeNode root2(2);
    TreeNode root3(3);
    root1.left  = &root2;
    root1.right = &root3;
    showVec(Solution2().inorderTraversal(&root1));

    return 0;
}



