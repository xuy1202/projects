/*
 *  Implement strStr().
 *
 *  Returns the index of the first occurrence of needle in haystack, or -1 if needle is not part of haystack.
 *
 *  Update (2014-11-02):
 *  The signature of the function had been updated to return the index instead of the pointer. If you still see your function signature returns a char * or String, please click the reload button to reset your code definition. 
 */

#include<iostream>
#include<string.h>


class Solution {
    public:
        int strStr(const char *haystack, const char *needle) {
            static struct _isSame{
                bool operator() (const char* lh
                            , const char*  rh
                            , int len ){
                    for(auto i=0; i<len; ++i){
                        if(*(lh+i) == 0x0){
                            return false;
                        }
                        if(*(rh+i) == 0x0){
                            return false;
                        }
                        if(*(lh+i) == *(rh+i)){
                            continue;
                        }
                        return false;
                    }
                    return true;
                }
            } isSame;

            const char* s = haystack;
            auto hlen = strlen(haystack);
            auto nlen = strlen(needle);

            do{
                if(s-haystack > hlen-nlen){
                    break;
                }
                if(isSame(s, needle, nlen)){
                    return s-haystack;
                }
            }while(*(++s));

            return -1;
        }
};

int main(void)
{
    std::cout << Solution().strStr("hello world", "hello") << std::endl;
    std::cout << Solution().strStr("hello world", "world") << std::endl;
    std::cout << Solution().strStr("hello world", "worldd") << std::endl;
    std::cout << Solution().strStr("", "worldd") << std::endl;
    std::cout << Solution().strStr("hello world", "") << std::endl;
    std::cout << Solution().strStr("", "") << std::endl;
    return 0;
}


