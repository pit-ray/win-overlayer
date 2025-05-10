#include "screen_overlay.hpp"

#include <iostream>


int main()
{
    using namespace screen_overlay ;

    ScreenOverlay so(L"Demo") ;
    if(!so.init()) {
        std::cout << "initialize failed\n" ;
        return 0 ;
    }

    while(so.update()) {
        Sleep(50) ;
    }
    std::cout << so.test() << std::endl ;

    return 0 ;
}
