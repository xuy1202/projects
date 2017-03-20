/*
 * Given a linked list, swap every two adjacent nodes and return its head.
 *
 * For example,
 * Given 1->2->3->4, you should return the list as 2->1->4->3.
 *
 * Your algorithm should use only constant space. You may not modify the values in the list, only nodes itself can be changed.
 */

#include "helper.h"


class Solution {
    private:
        void swap(int* lh, int* rh)
        {
            *lh = (*lh) ^ (*rh);
            *rh = (*lh) ^ (*rh);
            *lh = (*lh) ^ (*rh);
        }

    public:
        ListNode *swapPairs(ListNode *head) {
            ListNode* curr = head;

            while(curr){
                if(! curr      ) break;
                if(! curr->next) break;
                ListNode* nth1 = curr;
                ListNode* nth2 = curr->next;
                swap(&(nth1->val), &(nth2->val));
                curr = nth2->next;
            }

            return head;
        }
};



int main(void)
{
    ListNode n1(1);
    ListNode n2(2);
    ListNode n3(3);
    ListNode n4(4);

    n1.next = &n2;
    n2.next = &n3;
    //n3.next = &n4;

    showList(&n1);

    showList(Solution().swapPairs(&n1));

    return 0;
}



