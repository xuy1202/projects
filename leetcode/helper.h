
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <string.h>
#include <algorithm>
#include <stack>
#include<limits.h>


struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};



/* Definition for binary tree */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};


/* Definition for binary tree with next pointer. */
struct TreeLinkNode {
    int val;
    TreeLinkNode *left, *right, *next;
    TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
};


void showArr(int A[], int len)
{
    std::cout << "[";
    for(auto i=0; i<len; ++i){
        if(i != 0){
            std::cout << ", ";
        }
        std::cout << A[i];
    }
    std::cout << "]";
    std::cout << std::endl;
}


template<class T>
void showVec(const std::vector<T> v)
{
    std::cout << "[";
    for(auto it=v.begin(); it!=v.end(); ++it){
        if(it != v.begin()){
            std::cout << ", ";
        }
        std::cout << *it;
    }
    std::cout << "]";
    std::cout << std::endl;
}

template<class T>
void showSet(const std::set<T> v)
{
    std::cout << "[";
    for(auto it=v.begin(); it!=v.end(); ++it){
        if(it != v.begin()){
            std::cout << ", ";
        }
        std::cout << *it;
    }
    std::cout << "]";
    std::cout << std::endl;
}


void showList(const ListNode* head)
{
    const ListNode* curn = head;
    while(curn){
        std::cout << curn->val << " ";
        curn = curn->next;
    }
    std::cout << std::endl;
}

template<class KT, class VT>
void showMap(const std::map<KT, VT>& m)
{
    std::cout << ">>> Map:" << std::endl;
    for(auto it=m.begin(); it!=m.end(); ++it){
        std::cout << it->first << " " << it->second << std::endl;
    }
    std::cout << std::endl;
}


void inorderTraversal(TreeNode* root, std::vector< std::vector<int> >& ret, int level=1)
{
    if(level > ret.size()){
        ret.push_back(std::vector<int>());
    }
    if(root == NULL){
        ret[level-1].push_back(-1);
        return;
    }
    inorderTraversal(root->left, ret, level+1);
    ret[level-1].push_back(root->val);
    inorderTraversal(root->right, ret, level+1);
    return;
}


void showBtree(TreeNode *root)
{
    std::vector< std::vector<int> > ret;
    inorderTraversal(root, ret);
    std::vector<std::string> outs;
    int level = 0;
    for(auto it=ret.begin(); it!=ret.end(); ++it){
        // outs extend
        //for(auto it=outs.begin(); it!=outs.end(); ++it){
        //    for(auto i=0; i<=level; ++i){
        //        *it = " " + *it;
        //    }
        //}
        // new line
        std::ostringstream oss;
        for(auto jt=it->begin(); jt!=it->end(); ++jt){
            for(auto i=0; i<ret.size()-level; ++i){ oss << "   "; }
            oss << *jt;
            //for(auto i=0; i<=ret.size()-level; ++i){ oss << " "; }
        }
        level += 1;
        outs.push_back(oss.str());
    }
    for(auto it=outs.begin(); it!=outs.end(); ++it){
        std::cout << *it << std::endl;
    }
}


