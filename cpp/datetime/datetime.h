#ifndef DATETIME_H__
#define DATETIME_H__

#include <sys/time.h>
#include <time.h>
#include <inttypes.h>
#include <iostream>


#define NS_BEG_DATETIME__ namespace datetime {
#define NS_END_DATETIME__ }


NS_BEG_DATETIME__

//struct timezone{
//    int tz_minuteswest;  // 格林威治时间差
//    int tz_dsttime;      // DST 时间的修正方式
//}
//struct timeval{
//    long int tv_sec;     // 秒数
//    long int tv_usec;    // 微秒数
//}


class Datetime{
    public:
        Datetime(int64_t timestamp=-1)
            : timestamp_(timestamp), ltime_p_(NULL)
        {
            gettimeofday(&tv_, &tz_);
            if(timestamp_ > 0){
                tv_.tv_sec == timestamp_;
            }
        }

    public:
        uint64_t timestamp() const { return tv_.tv_sec;  }
        uint64_t u_secs()    const { return tv_.tv_usec; }
        uint64_t m_secs()    const { return tv_.tv_usec/1000; }

        uint64_t all_usecs() const {
            uint64_t cs = static_cast<uint64_t>(tv_.tv_sec * 1000000 + tv_.tv_usec);
            return cs;
        }
        uint64_t all_msecs() const {
            uint64_t cs = static_cast<uint64_t>(tv_.tv_sec * 1000 + tv_.tv_usec/1000);
            return cs;
        }

    public:
        uint32_t year()
        {
            init_localtime();
            return 1900 + ltime_.tm_year;
        }
        uint32_t month()
        { 
            init_localtime();
            return 1 + ltime_.tm_mon;
        }
        uint32_t day()
        { 
            init_localtime();
            return ltime_.tm_mday;
        }
        uint32_t hour()
        { 
            init_localtime();
            return ltime_.tm_hour;
        }
        uint32_t minute()
        { 
            init_localtime();
            return ltime_.tm_min;
        }
        uint32_t second()
        { 
            init_localtime();
            return ltime_.tm_sec;
        }

    public:
        int zone_delta()  const { return tz_.tz_minuteswest; }
        int zone_method() const { return tz_.tz_dsttime;     }
    
    public:
        inline void strftime(const std::string& format, std::string& out)
        {
            init_localtime();
            char buf[64];
            size_t s = ::strftime(buf, 64, format.c_str(), &ltime_);
            out.assign(buf, s);
        }

        inline std::string strftime(const std::string& format="%Y-%m-%d %H:%M:%S")
        {
            std::string out;
            strftime(format, out);
            return out;
        }

    private:
        inline void init_localtime()
        {
            if(timestamp_ < 0){
                time(&timestamp_);
            }
            // now we using ltime_ rather than original _p
            //    cause _p is static, it'll confused 
            //    when multi-thread env or multi-persistent-Datetime.
            //    And if multi-thread, you should lock outside by yourself
            if(ltime_p_ == NULL){
                ltime_p_ = localtime(&timestamp_);
                ltime_ = *ltime_p_;
            }
        }

    private:
        struct timeval  tv_;
        struct timezone tz_;
        time_t   timestamp_;
        struct tm    ltime_;
        struct tm* ltime_p_;
};


NS_END_DATETIME__


#endif // DATETIME_H__


