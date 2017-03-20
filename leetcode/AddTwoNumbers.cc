/*
 * You are given two linked lists representing two non-negative numbers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.
 *
 * Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
 * Output: 7 -> 0 -> 8
 */

#include "helper.h"


class Solution {
    public:
        ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
            ListNode* head = NULL;
            ListNode* next = NULL;
            bool adds_next = false;
            while(l1 || l2){
                int v1 = l1 ? l1->val: 0;
                int v2 = l2 ? l2->val: 0;
                int next_val = adds_next? v1+v2+1: v1+v2;
                if(! head){
                    head = new ListNode(next_val);
                    next = head;
                }else{
                    next->next = new ListNode(next_val);
                    next = next->next;
                }
                if(next->val > 9){
                    next->val = next->val - 10;
                    adds_next = true;
                }else{
                    adds_next = false;
                }
                l1 = l1? l1->next: NULL;
                l2 = l2? l2->next: NULL;
            }
            if(adds_next) next->next = new ListNode(1);
            return head;
        }
};

int main(void)
{
    ListNode n2(2);
    ListNode n4_1(4);
    ListNode n3(3);

    ListNode n5(5);
    ListNode n6(6);
    ListNode n4_2(4);

    ListNode n1(1);
    ListNode n8(8);
    ListNode n0(0);

    n1.next = &n8;

    showList(Solution().addTwoNumbers(&n0, &n1));

    showList(Solution().addTwoNumbers(&n4_1, &n5));
    showList(Solution().addTwoNumbers(&n5, &n5));

    n2.next = &n4_1;
    n4_1.next = &n3;

    n5.next = &n6;
    n6.next = &n4_2;

    showList(Solution().addTwoNumbers(&n2, &n5));

    return 0;
}


