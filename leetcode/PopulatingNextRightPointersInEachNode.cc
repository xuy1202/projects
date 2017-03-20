/*
 * Given a binary tree
 *
 *     struct TreeLinkNode {
 *        TreeLinkNode *left;
 *        TreeLinkNode *right;
 *        TreeLinkNode *next;
 *     }
 *  Populate each next pointer to point to its next right node. If there is no next right node, the next pointer should be set to NULL.
 *
 *  Initially, all next pointers are set to NULL.
 *
 *  Note:
 *
 *      You may only use constant extra space.
 *      You may assume that it is a perfect binary tree (ie, all leaves are at the same level, and every parent has two children).
 *      For example,
 *      Given the following perfect binary tree,
 *              1
 *             / \
 *            2   3
 *           / \ / \
 *          4  56   7
 *      After calling your function, the tree should look like:
 *           1 -> NULL
 *          / \
 *         2 -> 3 -> NULL
 *        / \  / \
 *       4->5->6->7 -> NULL
 */

#include "helper.h"


class Solution {
    public:
        void connect(TreeLinkNode *root)
        {
            if(root == NULL){
                return;
            }
            std::vector<TreeLinkNode*> vec;
            std::vector<TreeLinkNode*> tmp;
            vec.push_back(root);
            do{
                if(vec[0] == NULL){
                    break;
                }
                tmp.clear();
                for(auto i=0; i<vec.size()-1; ++i){
                    vec[i]->next = vec[i+1];
                    //std::cout << "Link: " << vec[i]->val << "->" << vec[i+1]->val << std::endl;
                    if(vec[i]->left && vec[i]->right){
                        tmp.push_back(vec[i]->left);
                        tmp.push_back(vec[i]->right);
                    }
                }
                vec[vec.size()-1]->next = NULL;
                tmp.push_back(vec[vec.size()-1]->left);
                tmp.push_back(vec[vec.size()-1]->right);
                vec.swap(tmp);
            }while(true);
        }
};


int main(void)
{
    TreeLinkNode n1(1);
    TreeLinkNode n2(2);
    TreeLinkNode n3(3);
    TreeLinkNode n4(4);
    TreeLinkNode n5(5);
    TreeLinkNode n6(6);
    TreeLinkNode n7(7);

    n1.left  = &n2;
    n1.right = &n3;

    n2.left  = &n4;
    n2.right = &n5;

    n3.left  = &n6;
    n3.right = &n7;

    Solution().connect(&n1);

    return 0;
}


