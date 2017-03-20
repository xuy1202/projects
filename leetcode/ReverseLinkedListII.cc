/**
 *  Definition for singly-linked list.
 *  struct ListNode {
 *      int val;
 *      ListNode *next;
 *      ListNode(int x) : val(x), next(NULL) {}
 *  };
 *
 *  Reverse a linked list from position m to n. Do it in-place and in one-pass.
 *
 *  For example:
 *  Given 1->2->3->4->5->NULL, m = 2 and n = 4,
 *
 *  return 1->4->3->2->5->NULL.
 *
 *  Note:
 *  Given m, n satisfy the following condition:
 *  1 ≤ m ≤ n ≤ length of list.
 **/

#include "helper.h"

class Solution {
    public:
        ListNode* reverseList(ListNode* head) {
            ListNode* last = NULL;
            ListNode* curr = head;
            while(curr){
                auto temp = curr->next;
                curr->next = last;
                last = curr;
                curr = temp;
            }
            return last;
        }

        // 1    2    [3    4    5]    6    7
        //     l1    l2         l3    l4
        ListNode* reverseBetween(ListNode* head, int m, int n)
        {
            if(m <= 0 || m > n) return NULL;
            ListNode* l1 = NULL;
            ListNode* l2 = NULL;
            ListNode* l3 = NULL;
            ListNode* l4 = NULL;
            ListNode* retl = NULL;
            ListNode* curr = head;
            size_t i = 0;
            while(++i){
                if(i == m-1){ l1 = curr; }
                if(i == m  ){ l2 = curr; }
                if(i == n  ){
                    l3 = curr;
                    l4 = curr->next;
                    break;
                }
                curr = curr->next;
            }
            std::cout << (l1?l1->val:-1) << "\t" << l2->val << "\t" << l3->val << "\t" << (l4?l4->val:-1) << std::endl;
            ListNode* tm = l2;
            l3->next = NULL;
            if(l1){
                l1->next = reverseList(l2);
                retl = head;
            }else{
                retl = reverseList(l2);
            }
            tm->next = l4;
            return retl;
        }
};


int main()
{
    auto node = new ListNode(1);
    node->next = new ListNode(2);
    node->next->next = new ListNode(3);
    //node->next->next->next = new ListNode(4);
    //node->next->next->next->next = new ListNode(5);
    showList(node);
    //showList(Solution().reverseBetween(node, 1, 4));
    //showList(Solution().reverseBetween(node, 1, 5));
    //showList(Solution().reverseBetween(node, 2, 4));
    //showList(Solution().reverseBetween(node, 2, 5));

    showList(Solution().reverseBetween(node, 3, 3));

    //auto node5 = new ListNode(5);
    //showList(node5);
    //showList(Solution().reverseBetween(node5, 1, 1));
    return 0;
}

