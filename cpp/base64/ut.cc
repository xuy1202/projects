
#include "base64.h"


int main(void)
{
    std::string a = "asdfasdfasdfasdf";
    std::cout << base64::encode(a) << std::endl;

    std::string b;
    base64::encode(a, b);
    std::cout << (base64::decode(b) == a) << std::endl;

    return 0;
}

