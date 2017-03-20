/*
 * Given a linked list, reverse the nodes of a linked list k at a time and return its modified list.
 *
 * If the number of nodes is not a multiple of k then left-out nodes in the end should remain as it is.
 *
 * You may not alter the values in the nodes, only nodes itself may be changed.
 *
 * Only constant memory is allowed.
 *
 * For example,
 * Given this linked list: 1->2->3->4->5
 *
 * For k = 2, you should return: 2->1->4->3->5
 *
 * For k = 3, you should return: 3->2->1->4->5
 */

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

        ListNode* reverseKGroup(ListNode* head, int k)
        {
            if(! head || k <=1) return head;
            ListNode* curr = head;
            size_t i = 0;
            while(curr && ++i){
                if(i == k  ){
                    ListNode* l3 = curr;
                    ListNode* l4 = curr->next;
                    l3->next = NULL;
                    ListNode* old_head = head;
                    head = reverseList(head);
                    old_head->next = reverseKGroup(l4, k);
                }
                curr = curr->next;
            }
            return head;
        }
};


int main()
{
    auto node = new ListNode(1);
    node->next = new ListNode(2);
    node->next->next = new ListNode(3);
    node->next->next->next = new ListNode(4);
    node->next->next->next->next = new ListNode(5);
    showList(node);

    showList(Solution().reverseKGroup(node, 3));

    return 0;
}

