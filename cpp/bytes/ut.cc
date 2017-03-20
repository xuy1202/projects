
#include "bytes_func.h"
#include <iostream>


int main(void)
{
    char b1[] = "www.baidu.com.";
    std::cout << b1 << std::endl;
    bytes::traversal_rotate_by_char(b1, 14, '.');
    std::cout << b1 << std::endl;

    char b2[] = "www.baidu.com.";
    std::cout << b2 << std::endl;
    bytes::traversal_rotate_by_char(b2, 14, '.');
    std::cout << b2 << std::endl;

    char b3[] = "www.baidu.com";
    std::cout << b3 << std::endl;
    bytes::traversal_rotate_by_char(b3, 13, '.');
    std::cout << b3 << std::endl;

    char b4[] = ".www.baidu.com.";
    std::cout << b4 << std::endl;
    bytes::traversal_rotate_by_char(b4, 15, '.');
    std::cout << b4 << std::endl;
    return 0;
}
