#ifndef H_BYTES_FUNC__
#define H_BYTES_FUNC__

#include "bytes.h"

#include <iostream>

NS_BEG_BYTES__


void reverse(char* buf, int len)
{
    int mid = len >> 1;
    for(int i=0; i<mid; ++i){
        char* lh = buf + i;
        char* rh = buf + len - 1 - i;
        *lh = (*lh) ^ (*rh);
        *rh = (*lh) ^ (*rh);
        *lh = (*lh) ^ (*rh);
    }
}


void reverse_until(char* buf, int max_len, char chr)
{
    int end = 0;
    while( (*(buf+end) != chr) && (end<max_len)) ++end;
    reverse(buf, end);
}


void rotate_by_index(char* buf, int len, int idx)
{
    if(idx >= len) return;
    reverse(buf, idx);
    reverse(buf+idx+1, len-idx-1);
    reverse(buf, len);
}


bool lotate_by_char(char* buf, int len, char chr)
{
    int idx = 0;
    while( (*(buf+idx) != chr) && (idx <= len) ) ++idx;
    if(idx == len){
        return false;
    }
    rotate_by_index(buf, len, idx);
    return true;
}


bool rotate_by_char(char* buf, int len, char chr)
{
    int idx = len-1;
    while( (*(buf+idx) != chr) && (idx >= -1) ) --idx;
    if(idx < 0){
        return false;
    }
    rotate_by_index(buf, len, idx);
    return true;
}


void split_reverse_by_char(char* buf, int len, char chr)
{
    int idx = 0;
    while( (*(buf+idx) != chr) && (idx < len) ) ++idx;
    if(idx > 0){
        reverse(buf, idx);
    }
    if(idx < len - 1){
        split_reverse_by_char(buf+idx+1, len-idx-1, chr);
    }
}


void traversal_rotate_by_char(char* buf, int len, char chr)
{
    split_reverse_by_char(buf, len, chr);
    reverse(buf, len);
}


//void traversal_rotate_by_char(char* buf, int len, char chr)
//{
//    bool head_reversed = false;
//    for(int i=0; i<len; ++i){
//        if(*(buf+i) != chr){
//            continue;
//        }
//        if((i>0) && (!head_reversed)){
//            reverse(buf, i);
//            head_reversed = true;
//        }
//        reverse_until(buf+i+1, len-i-1, chr);
//        head_reversed = true;
//    }
//    reverse(buf, len);
//}


NS_END_BYTES__


#endif // H_BYTES_FUNC__


