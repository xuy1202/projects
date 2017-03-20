
#include <typeinfo>
#include <stdio.h>
#include <iostream>
#include "yc.h"
#include <inttypes.h>



int main(void)
{

    char c = 'c';
    yc::print(c);
    yc::print(yc::str(c));

    std::cout << yc::str(c) << std::endl;
    int i = 0;
    std::cout << yc::str(i) << std::endl;
    unsigned u = 1;
    std::cout << yc::str(u) << std::endl;
    long l = 2;
    std::cout << yc::str(l) << std::endl;
    unsigned long ul = 3;
    std::cout << yc::str(ul) << std::endl;
    long long ll = 4;
    std::cout << yc::str(ll) << std::endl;
    unsigned long long ull = 5;
    std::cout << yc::str(ull) << std::endl;
    float f = 6.6;
    std::cout << yc::str(f) << std::endl;
    double d = 7.7;
    std::cout << yc::str(d) << std::endl;
    bool b1 = true;
    std::cout << yc::str(b1) << std::endl;
    bool b2 = false;
    std::cout << yc::str(b2) << std::endl;

    uint32_t u32 = 32;
    std::cout << yc::str(u32) << std::endl;

    return 0;
}

