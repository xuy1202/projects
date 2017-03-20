#include "cpplru.h"


#ifdef _CPPLRU_UT

int main(void)
{
    cpplru::LRUMap<std::string, std::string> m(3);
    m.show();
    m.set("a", "a");
    m.set("b", "b");
    m.show();
    m.set("c", "c");
    m.set("d", "d");
    m.show();
    m.set("e", "e");
    m.set("a", "a");
    m.show();
    m.set("a", "aa");
    m.set("d", "dd");
    m.show();
    //for(int i=0; i<1000000; ++i){
    //    m.set("a", "aaa");
    //}
    //m.show();
    std::string v;
    m.pop("d", v);
    std::cout << v << std::endl;
    m.show();
    m.pop("a", v);
    std::cout << v << std::endl;
    m.show();
    m.pop("e", v);
    std::cout << v << std::endl;
    m.show();
    std::cout << m.miss_count() << std::endl;
    std::cout << m.lost_count() << std::endl;
    return 0;
}

#endif


