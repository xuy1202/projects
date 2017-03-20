/*
 * Given a linked list, determine if it has a cycle in it.
 *
 * Follow up:
 * Can you solve it without using extra space?
 */
#include "helper.h"


class Solution {
    public:
        bool hasCycle(ListNode *head) {
            if(! head){
                return false;
            }
            ListNode* slow = head;
            ListNode* fast = head;
            do{
                if(! slow->next){
                    return false;
                }
                if(! fast->next || ! fast->next->next){
                    return false;
                }
                fast = fast->next->next;
                if(fast == slow || fast == slow->next){
                    return true;
                }
                slow = slow->next;
            }while(true);
        }
};


int main(void)
{
    ListNode n1 = ListNode(1);
    std::cout << Solution().hasCycle(NULL) << std::endl;
    std::cout << Solution().hasCycle(&n1) << std::endl;
    ListNode n2 = ListNode(2);
    n1.next = &n2;
    std::cout << Solution().hasCycle(&n1) << std::endl;
    n2.next = &n1;
    std::cout << Solution().hasCycle(&n1) << std::endl;

    ListNode n3 = ListNode(3);
    n1.next = &n2;
    n2.next = &n3;
    std::cout << Solution().hasCycle(&n1) << std::endl;

    n3.next = &n2;
    std::cout << Solution().hasCycle(&n1) << std::endl;

    n3.next = &n1;
    std::cout << Solution().hasCycle(&n1) << std::endl;

    return 0;
}


