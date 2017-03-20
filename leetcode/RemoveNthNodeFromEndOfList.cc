/*
 * Given a linked list, remove the nth node from the end of list and return its head.
 *
 * For example,
 *
 *    Given linked list: 1->2->3->4->5, and n = 2.
 *
 *       After removing the second node from the end, the linked list becomes 1->2->3->5.
 *
 *       Note:
 *       Given n will always be valid.
 *       Try to do this in one pass. 
 */

#include "helper.h"


class Solution {
    public:
        ListNode *removeNthFromEnd(ListNode *head, int n) {
            std::vector< ListNode* > indexs;
            for(auto i=0; i<=n; ++i) indexs.push_back(NULL);

            ListNode* lest = head;
            int i = 0;
            while(true){
                indexs[i % (n+1)] = lest;
                lest = lest->next;
                if(! lest){
                    break;
                }
                i = ++i % (n+1);
            }
            ListNode* last = indexs[(i+1)%(n+1)];
            // remove head
            if(last == NULL){
                return head->next;
            }else{
                ListNode* todl = indexs[(i+2)%(n+1)];
                last->next = todl->next;
                return head;
            }
        }
};


int main(void)
{
    ListNode n1(1);
    ListNode n2(2);
    ListNode n3(3);
    ListNode n4(4);
    ListNode n5(5);

    n1.next = &n2;
    n2.next = &n3;
    n3.next = &n4;
    n4.next = &n5;

    ListNode* cur = &n1;

    showList(cur = Solution().removeNthFromEnd(cur, 2));
    showList(cur = Solution().removeNthFromEnd(cur, 1));
    showList(cur = Solution().removeNthFromEnd(cur, 3));
    showList(cur = Solution().removeNthFromEnd(cur, 1));
    showList(cur = Solution().removeNthFromEnd(cur, 1));

    return 0;
}


