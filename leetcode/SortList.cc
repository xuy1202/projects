/*
 * Sort a linked list in O(n log n) time using constant space complexity.
 */

#include "helper.h"


class Solution {
    public:
        // extreaming env, time exceeded
        ListNode* quickSort(ListNode *head)
        {
            if(! head || ! head->next){
                return head;
            }
            ListNode* reth = head;
            ListNode* prev = head;
            ListNode* next = head->next;
            int sentry = head->val;
            while(next){
                if(next->val >= sentry){
                    next=next->next;
                    prev=prev->next;
                    continue;
                }
                prev->next = next->next;
                next->next = reth;
                reth = next;
                next = prev->next;
            }
            // split sort
            head->next = quickSort(head->next);
            next = head->next;
            head->next = NULL;
            reth = quickSort(reth);
            // re-join
            prev = reth;
            while(prev->next) prev = prev->next;
            prev->next = next;

            return reth;
        }

        ListNode* mergeSortedList(ListNode* lis1, ListNode* lis2)
        {
            ListNode* head = NULL;
            ListNode* tail = NULL;
            while(lis1 && lis2){
                ListNode* smll = NULL;
                if(lis1->val <= lis2->val){
                    smll = lis1;
                    lis1 = lis1->next;
                }else{
                    smll = lis2;
                    lis2 = lis2->next;
                }
                if(! head){
                    head = tail = smll;
                }else{
                    tail->next = smll;
                    tail = smll;
                }
            }
            if(lis1) tail->next = lis1;
            if(lis2) tail->next = lis2;
            return head;
        }

        ListNode* sortList(ListNode *head)
        {
            if(! head || ! head->next){
                return head;
            }
            if(! head->next->next){
                if(head->val > head->next->val){
                    ListNode* reth = head->next;
                    reth->next = head;
                    head->next = NULL;
                    return reth;
                }
                return head;
            }

            ListNode* slow = head;
            ListNode* fast = head;
            while(fast && fast->next){
                slow = slow->next;
                fast = fast->next->next;
            }
            ListNode* rh = sortList(slow->next);
            slow->next = NULL;
            ListNode* lh = sortList(head);
            return mergeSortedList(lh, rh);
        }

};


int main(void)
{
    ListNode n1(1);
    ListNode n2(2);
    ListNode n3(3);
    ListNode n4(4);
    ListNode n5(5);
    ListNode n6(6);

    // 6 1 3 5 2 4
    n6.next = &n1;
    n1.next = &n3;
    n3.next = &n5;
    n5.next = &n2;
    n2.next = &n4;

    showList(&n6);
    showList(Solution().sortList(&n6));

    // {4,19,14,5,-3,1,8,5,11,15}
    ListNode n51(5);
    ListNode n52(5);
    ListNode n8(8);
    ListNode n_3(-3);
    ListNode n19(19);
    ListNode n11(11);
    ListNode n14(14);
    ListNode n15(15);

    n4.next = &n19;
    n19.next = &n14;
    n14.next = &n51;
    n51.next = &n_3;
    n_3.next = &n1;
    n1.next = &n8;
    n8.next = &n52;
    n52.next = &n11;
    n11.next = &n15;

    showList(&n4);
    showList(Solution().sortList(&n4));

    return 0;
}



