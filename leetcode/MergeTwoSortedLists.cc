/*
 * Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes of the first two lists.
 */

#include "helper.h"


class Solution {
    public:
        ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
            if(l1 == NULL) return l2;
            if(l2 == NULL) return l1;

            ListNode* cur1 = l1;
            ListNode* cur2 = l2;
            ListNode* head;
            if(l1->val < l2->val){
                head = l1;
                cur1 = l1->next;
            }else{
                head = l2;
                cur2 = l2->next;
            }
            ListNode* last = head;

            while(cur1 || cur2){
                if(! cur1){ last->next = cur2; break; }
                if(! cur2){ last->next = cur1; break; }

                if(cur1->val <= cur2->val){
                    last->next = cur1;
                    cur1 = cur1->next;
                }else{
                    last->next = cur2;
                    cur2 = cur2->next;
                }
                last = last->next;
                last->next = NULL;
            }
            return head;
        }
};


int main(void)
{
    auto la = new ListNode(1);
    la->next = new ListNode(3);
    la->next->next = new ListNode(5);
    la->next->next->next = new ListNode(7);

    auto lb = new ListNode(2);
    lb->next = new ListNode(4);
    lb->next->next = new ListNode(6);
    lb->next->next->next = new ListNode(7);
    lb->next->next->next->next = new ListNode(9);

    showList(Solution().mergeTwoLists(la, lb));
    return 0;
}


