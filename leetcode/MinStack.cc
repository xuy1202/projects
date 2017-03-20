/*
 * Design a stack that supports push, pop, top, and retrieving the minimum element in constant time.
 *
 * push(x) -- Push element x onto stack.
 * pop() -- Removes the element on top of the stack.
 * top() -- Get the top element.
 * getMin() -- Retrieve the minimum element in the stack.
 */


#include "helper.h"


class MinStack {
    private:
        std::map<int, int> min_;
        std::vector<int> stack_;

    public:
        void push(int x) {
            stack_.push_back(x);
            min_[x] += 1;
        }

        void pop() {
            if(stack_.empty()){
                return;
            }
            auto i = min_.find(*(stack_.rbegin()));
            --(i->second);
            if(i->second == 0){
                min_.erase(i);
            }
            stack_.pop_back();
        }

        int top() {
            if(stack_.empty()){
                return INT_MIN;
            }
            return *(stack_.rbegin());
        }

        int getMin() {
            if(min_.empty()){
                return INT_MAX;
            }
            return min_.begin()->first;
        }
};


int main(void)
{
    MinStack s;
    // push(2),push(0),push(3),push(0),getMin,pop,getMin,pop,getMin,pop,getMin
    s.push(2);
    s.push(0);
    s.push(3);
    s.push(0);

    std::cout << s.getMin() << std::endl;
    s.pop();
    std::cout << s.getMin() << std::endl;
    s.pop();
    std::cout << s.getMin() << std::endl;
    s.pop();
    std::cout << s.getMin() << std::endl;

    return 0;
}


