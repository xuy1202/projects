/**
 *
 *  Definition for singly-linked list.
 *  struct ListNode {
 *      int val;
 *      ListNode *next;
 *      ListNode(int x) : val(x), next(NULL) {}
 *  };
 *
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
};

int main()
{
    auto node = new ListNode(1);
    node->next = new ListNode(2);
    node->next->next = new ListNode(3);
    node->next->next->next = new ListNode(4);
    showList(node);
    showList(Solution().reverseList(node));
    showList(Solution().reverseList(NULL));
    return 0;
}
