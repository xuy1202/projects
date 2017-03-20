#ifndef H_YME_HELPER__
#define H_YME_HELPER__

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <inttypes.h>
#include <assert.h>


#define NS_BEG_YME__ namespace yme {
#define NS_END_YME__ }

NS_BEG_YME__


template<class RawType, class DatType, class RuleType, class RetType>
class Engine;


void split_to_vector(const std::string& in                                                                                                                                         
            , const std::string& sep
            , std::vector<std::string>& v)
{
    std::string::size_type spos = 0;
    do{
        auto pos = in.find(sep, spos);
        if (pos == std::string::npos || pos >= in.size()){
            break;
        }
        v.push_back(in.substr(spos, pos-spos));
        spos = pos + sep.size();
    }while(1);
    v.push_back(in.substr(spos, in.size()-spos));
}


struct Logger{
    enum LogLevel{
        unknown = 0,
        kDebug  = 1,
        kInfo   = 2,
        kWarn   = 3,
        kError  = 4,
    };

    LogLevel                     LevelThreshold;
    typedef void (*LogFunc)(const std::string&);
    std::map<LogLevel, LogFunc>           rmap_;

    Logger(): LevelThreshold(unknown)
    {
    }

    void RegisterDebugFunc(LogFunc func) { rmap_[kDebug] = func; }
    void RegisterInfoFunc(LogFunc  func) { rmap_[kInfo]  = func; }
    void RegisterWarnFunc(LogFunc  func) { rmap_[kWarn]  = func; }
    void RegisterErrorFunc(LogFunc func) { rmap_[kError] = func; }

    void Logging(LogLevel level, const std::string& errmsg) const
    {
        if(level <= LevelThreshold){
            return;
        }
        auto it = rmap_.find(level);
        if(it == rmap_.end()){
            std::cerr << "YmeLog" << static_cast<unsigned>(level) << ": " << errmsg << std::endl;
        }else{
            (it->second)(errmsg);
        }
    }

    void Debug(const std::string& errmsg) const { Logging(kDebug, errmsg); }
    void Info( const std::string& errmsg) const { Logging(kInfo , errmsg); }
    void Warn( const std::string& errmsg) const { Logging(kWarn , errmsg); }
    void Error(const std::string& errmsg) const { Logging(kError, errmsg); }

};


NS_END_YME__


#endif // H_YME_HELPER__


