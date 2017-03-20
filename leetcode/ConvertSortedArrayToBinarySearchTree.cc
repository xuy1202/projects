/*
 * Given an array where elements are sorted in ascending order, convert it to a height balanced BST.
 */

#include "helper.h"


class Solution {
    public:
        TreeNode* addMiddle(std::vector<int>& num, size_t beg, size_t end)
        {
            size_t mid = beg + (end - beg)/2;
            if(mid >= num.size()){
                return NULL;
            }
            TreeNode* root = new TreeNode(num[mid]);
            root->left  = addMiddle(num, beg, mid-1);
            root->right = addMiddle(num, mid+1, end);
            return root;
        }

        TreeNode* sortedArrayToBST(std::vector<int>& num)
        {
            return addMiddle(num, 0, num.size()-1);
        }
};


int main(void)
{
    std::vector<int> l = {1,2,3,4,5};
    Solution().sortedArrayToBST(l);

    return 0;
}


