
#include "hashlib.h"

int main(void)
{
    std::cout << (hashlib::md5sum("asdf") == "912ec803b2ce49e4a541068d495ab570") << std::endl; 
    std::cout << (hashlib::md5sum("")     == "d41d8cd98f00b204e9800998ecf8427e") << std::endl; 
    return 0;
}


