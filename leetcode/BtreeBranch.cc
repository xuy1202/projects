
#include "helper.h"

struct CumuStat{
    int  meet;
    bool stop;
    bool match;
    std::vector<int>* vec;

    CumuStat(std::vector<int>* v):meet(0), stop(false), match(false), vec(v){
    }

    bool operator ()(int v, int pos=0)
    {
        // finding can be optimized by pre-formated char[] indexing
        if(std::find(vec->begin(), vec->end(), v) != vec->end()){
            ++meet;
            return true;
        }
        return false;
    }

    void finish(bool mm)
    {
        if(meet == 0){
            stop = false;
        }else if(meet == 1 && vec->size() == 1){
            stop = true;
            match = true;
        }else if(meet == vec->size()){
            stop = true;
            match = mm;
        }else{
            if(mm){
                stop = false;
            }else{
                stop = true;
            }
        }
    }
};


void m_traverse(TreeNode* root, CumuStat& stat)
{
    if(! root) return;

    if(stat.stop) return; m_traverse(root->left, stat);
    if(stat.stop) return; bool mm = stat(root->val);
    if(stat.stop) return; m_traverse(root->right, stat);
    stat.finish(mm);
}


bool in_branch(TreeNode* root, std::vector<int>& list)
{
    CumuStat stat(&list);
    m_traverse(root, stat);
    return stat.match;
}


int main(void)
{
    auto root = new TreeNode(6);
    root->left = new TreeNode(4);
    root->right = new TreeNode(8);
    root->left->left = new TreeNode(2);
    root->left->right = new TreeNode(5);
    root->left->left->left = new TreeNode(1);
    root->left->left->right = new TreeNode(3);

    std::vector<int> v;
    v = {1};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {5};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {6};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {8};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {1,2,3};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {1,2,4};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {2,3,4};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {2,3,4,5};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {2,4,6,8};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {5,4,6,8};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    std::cout << "======fail" << std::endl;
    v = {5, 6};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {1,4};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {3,4};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {4,8};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {1,2,4,8};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {4,5,1};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {45};
    showVec(v); std::cout << in_branch(root, v) << std::endl;
    v = {45,6};
    showVec(v); std::cout << in_branch(root, v) << std::endl;

    return 0;
}


