/*
 *  Given a sorted linked list, delete all duplicates such that each element appear only once.
 *
 *  For example,
 *  Given 1->1->2, return 1->2.
 *  Given 1->1->2->3->3, return 1->2->3. 
 */

#include <iostream>


struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};


class Solution {
    public:
        ListNode *deleteDuplicates(ListNode *head) {
            ListNode* rlist = NULL;
            ListNode* rlast = NULL;
            ListNode* llist = head;
            while(llist){
                if(rlast == NULL){
                    rlast = new ListNode(llist->val);
                    rlist = rlast;
                }else if(rlast->val != llist->val){
                    ListNode* node= new ListNode(llist->val);
                    rlast->next = node;
                    rlast = node;
                }else{
                    ;
                }
                llist = llist->next;
            }
            return rlist;
        }
};


int main(void)
{
    ListNode n1(1);
    ListNode n11(1);
    ListNode n2(2);
    ListNode n3(3);
    ListNode n33(3);

    n1.next = &n11;
    n11.next = &n2;
    n2.next = &n3;
    n3.next = &n33;

    ListNode *list = &n1;
    while(list){
        std::cout << list->val << " ";
        list = list->next;
    }
    std::cout << std::endl;

    ListNode* r = Solution().deleteDuplicates(&n1);
    list = r;
    while(list){
        std::cout << list->val << " ";
        list = list->next;
    }
    std::cout << std::endl;
    return 0;
}


