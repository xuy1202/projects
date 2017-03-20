#ifndef H_XEVENT__
#define H_XEVENT__

#include <libev/ev.h>
#include <iostream>

#define NS_BEG_XEVENT__ namespace xevent {
#define NS_END_XEVNET__ }


NS_BEG_XEVENT__

extern struct ev_loop* loop;


extern struct _gLoopIns{
    bool started;

    _gLoopIns();
    void start();
} LoopInstance;


template<class WatcherType>
struct Event{
    typedef void (*CallBack)(void);
    template<CallBack cb>
        static void cb_(struct ev_loop* loop, WatcherType* watch, int evt)
        {
            (*cb)();
        }
};

struct EventPeriodic{
    struct ev_periodic watcher_;

    template<Event<struct ev_periodic>::CallBack cb>
        void set(double interval, double at=0.0)
        {
            ev_periodic_init(&watcher_, Event<struct ev_periodic>::cb_<cb>, at, interval, 0);
            ev_periodic_start(loop, &watcher_);
        }
};

struct EventTimer{
    struct ev_timer watcher_;

    template<Event<struct ev_timer>::CallBack cb>
        void set(int repeat, double at=0.0)
        {
            ev_timer_init(&watcher_, Event<struct ev_timer>::cb_<cb>, at, repeat);
            ev_timer_start(loop, &watcher_);
        }

};


NS_END_XEVNET__


#endif // H_XEVENT__

