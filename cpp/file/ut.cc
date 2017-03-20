
#include "file.h"

int main(void)
{
    std::string s;
    if(file::php::file_get_contents("/etc/passwd", s)){
        std::cout << s << std::endl;
    }
    std::string o = "passwd.bak";
    if(file::php::file_put_contents(o, s, std::ios::binary|std::ios::app)){
        std::cout << "saved in " << o << std::endl;
    }
    return 0;
}
