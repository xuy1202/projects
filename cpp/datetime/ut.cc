#include "datetime.h"
#include<iostream>


int main(void)
{
    datetime::Datetime n;
    std::cout << n.timestamp() << std::endl;

    std::cout << n.year()   << std::endl;
    std::cout << n.month()  << std::endl;
    std::cout << n.day()    << std::endl;
    std::cout << n.hour()   << std::endl;
    std::cout << n.minute() << std::endl;
    std::cout << n.second() << std::endl;
    std::cout << std::endl;
    std::cout << n.u_secs() << std::endl;
    std::cout << n.m_secs() << std::endl;

    std::cout << n.zone_delta()  << std::endl;
    std::cout << n.zone_method() << std::endl;

    std::cout << n.strftime("%Y|%m|%d--%H%M%S") << std::endl; 
    std::cout << n.strftime()                   << std::endl; 

    datetime::Datetime n2(1000000000);
    std::cout << n2.strftime() << std::endl;

    return 0;
}


