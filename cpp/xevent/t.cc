
#include "xevent.h"


void pcb(void) { std::cout << "p" << std::endl; }
void tcb(void) { std::cout << "t" << std::endl; }


int main() 
{
    //struct ev_loop *loop = ev_default_loop(0);
    //struct ev_periodic periodic_watcher;

    //ev_periodic_init(&periodic_watcher, cb, 0.0, 1, 0);
    //ev_periodic_start(loop, &periodic_watcher);

    //while (1) {
    //    ev_loop(loop, 0);
    //}

    xevent::EventPeriodic p;
    p.set<pcb>(1.0);

    xevent::EventTimer t;
    t.set<tcb>(5, 3.0);

    xevent::LoopInstance.start();

    return 0;
}

