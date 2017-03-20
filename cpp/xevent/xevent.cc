
#include "xevent.h"


NS_BEG_XEVENT__

struct ev_loop*  loop;
struct _gLoopIns LoopInstance;


_gLoopIns::_gLoopIns(): started(false)
{
    loop = ev_default_loop(0);
}


void _gLoopIns::start()
{
    if(started){
        return;
    }
    while (1) {
        ev_loop(loop, 0);
    }
}


NS_END_XEVNET__

