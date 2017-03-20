#ifndef H_JGROUP__
#define H_JGROUP__

#include <yme/bits.hpp>
#include <yme/proto.hpp>


#define _YME_VAL_ITEM_SPLITOR ";"
#define _YME_VAL_SPAN_SPLITOR "-"


NS_BEG_YME__


struct Judger{
    bits::BitsMap                      rids;
    const bits::BitsMap*               omit;
    std::map<DataItem, bits::BitsMap> cache;
    
    Judger(): omit(NULL)
    {
    
    }

    inline void RegisterOmits(const bits::BitsMap* o)
    {
        omit = o;
    }

    inline bool Feed(uint64_t idx, const std::string& val)
    {
        if(feed(idx, val)){
            rids.open(idx);
            return true;
        }
        return false;
    }

    struct OpensMapper: bits::BitsMap::Mapper{
        bits::BitsMap*       bmap;
        const bits::BitsMap* cmap;

        void operator () (uint64_t idx) const
        {
            if(cmap->is_open(idx)){
                bmap->open(idx);
            }
        }
    };

    inline bits::BitsMap Match(const DataItem& item 
                , const bits::BitsMap* const c=NULL)
    {
        if(c == NULL){
            auto it = cache.find(item);
            if(it != cache.end()){
                return it->second;
            }
        }

        const bits::BitsMap* conmap = (c? c:&rids);
        bits::BitsMap retmap;
        uint64_t w = match(retmap, item, conmap);
        // some rule don't have some conds
        //   so we deem it as matched
        if(omit){
            OpensMapper mapper;
            mapper.cmap =  conmap;
            mapper.bmap = &retmap;
            omit->map(&mapper);
            w += omit->opens();
        }

        //std::cout << "W: " << item.str() << " " << w << std::endl;
        // bigger w means heavy matching process
        //if(c == NULL && w >= 1){
        //    cache[item] = retmap;
        //}

        return retmap;
    }

    virtual bool feed(uint64_t idx, const std::string& val) = 0;

    virtual uint64_t match(bits::BitsMap& retmap
                , const DataItem& item 
                , const bits::BitsMap* const conmap=NULL) const = 0;
};



struct FilterMapper: bits::BitsMap::Mapper{
    const std::set<uint64_t>* s;
    bits::BitsMap*            b;

    void operator () (uint64_t idx) const
    {
        if(s->find(idx) != s->end()) b->open(idx);
    }
};

/* ================================================================
 * Item Judger, support number(int64_t) && string 
 * ================================================================
 */

template<class KT, class VT>
inline uint64_t SingleMatch(const std::map<KT, VT>& itmmap_
            , const KT& pot
            , const bits::BitsMap& conmap
            , bits::BitsMap& retmap
            )
{
    auto it = itmmap_.find(pot);
    if(it == itmmap_.end()){
        return 0;
    }
    if(it->second.size() < conmap.opens()){
        for(auto jt=it->second.begin()
                    ; jt!=it->second.end()
                    ; ++jt){
            uint32_t idx = *jt;
            if(conmap.is_close(idx)){
                continue;
            }
            retmap.open(idx);
        }
    }else{
        FilterMapper mapper;
        mapper.s = &(it->second);
        mapper.b = &(retmap); 
        conmap.map(&mapper);
    }
    uint64_t w = (it->second.size() < conmap.opens()
                ? it->second.size(): conmap.opens());
    return w;
}


struct NumberItemJudger: Judger{
    std::map<int64_t, std::set<uint64_t> > itmmap_;

    bool feed(uint64_t idx, const std::string& val)
    {
        int64_t num = strtoll(val.c_str(), NULL, 10);
        itmmap_[num].insert(idx);
        return true;
    }

    uint64_t match(bits::BitsMap& retmap
                , const DataItem& item 
                , const bits::BitsMap* const conmap=NULL) const
    {
        int64_t pot = item.value.int64t_value;
        return SingleMatch(itmmap_, pot, *conmap, retmap);
    }
};


struct StringItemJudger: Judger{
    std::map<std::string, std::set<uint64_t> > itmmap_;

    bool feed(uint64_t idx, const std::string& val)
    {
        itmmap_[val.c_str()].insert(idx);
        return true;
    }

    uint64_t match(bits::BitsMap& retmap
                , const DataItem& item 
                , const bits::BitsMap* const conmap=NULL) const
    {
        const std::string& pot = *(item.value.str_ptr);
        return SingleMatch(itmmap_, pot, *conmap, retmap);
    }
};


/* ================================================================
 * Span Judger, support number(int64_t) && double
 * ================================================================
 */


template<class KT, class VT>
inline uint64_t DuldirMatch(const std::map<KT, VT>& begmap_
            , const std::map<KT, VT>& endmap_
            , const KT& pot
            , const bits::BitsMap& conmap
            , bits::BitsMap& retmap
            )
{
    uint64_t w = 0;

    bits::BitsMap tmpmap;
    auto bit = begmap_.upper_bound(pot);
    for(auto it=begmap_.begin(); it!=bit; ++it){
        if(it->second.size() < conmap.opens()){
            for(auto jt=it->second.begin()
                        ; jt!=it->second.end()
                        ; ++jt){
                uint32_t idx = *jt;
                if(conmap.is_close(idx)){
                    continue;
                }
                tmpmap.open(idx);
            }
        }else{
            FilterMapper mapper;
            mapper.s = &(it->second);
            mapper.b = &(tmpmap); 
            conmap.map(&mapper);
        }
        w += (it->second.size() < conmap.opens()
                ? it->second.size(): conmap.opens());
    }
    auto eit = endmap_.lower_bound(pot);
    for(auto it=eit; it!=endmap_.end(); ++it){
        if(it->second.size() < 10){
            for(auto jt=it->second.begin()
                        ; jt!=it->second.end()
                        ; ++jt){
                uint32_t idx = *jt;
                if(tmpmap.is_open(idx)){
                    retmap.open(idx);
                }
            }
        }else{
            FilterMapper mapper;
            mapper.s = &(it->second);
            mapper.b = &(retmap); 
            conmap.map(&mapper);
        }
        w += (it->second.size() < conmap.opens()
                ? it->second.size(): conmap.opens());
    }
    return w;
}


struct NumberSpanJudger: Judger{
    std::map<int64_t, std::set<uint64_t> > begmap_;
    std::map<int64_t, std::set<uint64_t> > endmap_;

    // like: 1-9
    bool feed(uint64_t idx, const std::string& val)
    {
        std::vector<std::string> vec;
        split_to_vector(val, _YME_VAL_SPAN_SPLITOR, vec); 
        if(vec.size() != 2){
            return false;
        }
        int64_t beg = strtoll(vec[0].c_str(), NULL, 10);
        int64_t end = strtoll(vec[1].c_str(), NULL, 10);
        begmap_[beg].insert(idx);
        endmap_[end].insert(idx);
        return true;
    }

    uint64_t match(bits::BitsMap& retmap
                , const DataItem& item 
                , const bits::BitsMap* const conmap=NULL) const
    {
        assert(item.type == DataItem::kNumber);
        int64_t pot = item.value.int64t_value;
        return DuldirMatch(begmap_, endmap_, pot, *conmap, retmap);
    }
};


struct DoubleSpanJudger: Judger{
    std::map<double, std::set<uint64_t> > begmap_;
    std::map<double, std::set<uint64_t> > endmap_;

    // like: 1.2-3.4
    bool feed(uint64_t idx, const std::string& val)
    {
        std::vector<std::string> vec;
        split_to_vector(val, _YME_VAL_SPAN_SPLITOR, vec); 
        if(vec.size() != 2){
            return false;
        }
        double beg = atof(vec[0].c_str());
        double end = atof(vec[1].c_str());
        begmap_[beg].insert(idx);
        endmap_[end].insert(idx);
        return true;
    }

    uint64_t match(bits::BitsMap& retmap
                , const DataItem& item 
                , const bits::BitsMap* const conmap=NULL) const
    {
        assert(item.type == DataItem::kDouble);
        double pot = item.value.double_value;
        return DuldirMatch(begmap_, endmap_, pot, *conmap, retmap);
    }
};


/* ================================================================
 * FitFunc Judger, support double 
 * ================================================================
 */


//struct FitFuncJudger: Judger{
//    double ldiff;
//    double rdiff;
//
//    inline bool InSpan(double t, double z)
//    {
//        if(t < ldiff * z) return false;
//        if(t > rdiff * z) return false;
//        return true;
//    }
//
//    bool feed(uint64_t idx, const std::string& val)
//    {
//        return true;
//    }
//
//    void match(bits::BitsMap& retmap
//                , const DataItem& item 
//                , const bits::BitsMap* const conmap=NULL) const
//    {
//        //double pot = item.value.double_value;
//    }
//};
//
//
//struct FitFunc1Judger: public FitFuncJudger{
//    virtual double Calc(double x) = 0;
//
//    //bool match(double x, double y)
//    //{
//    //    double _y = Calc(x);
//    //    return InSpan(_y, y);
//    //}
//};
//
//
//struct FitFunc2Judger: public FitFuncJudger{
//    virtual double Calc(double x, double y) = 0;
//
//    //bool match(double x, double y, double z)
//    //{
//    //    double _z = Calc(x, y);
//    //    return InSpan(_z, z);
//    //}
//};
//
//
//struct FitFunc11Judger: FitFunc1Judger{
//    // a+b*x=cy-dy # c,d for diff
//    double a;
//    double b;
//
//    inline double Calc(double x)
//    {
//        return a + b*x;
//    }
//
//    bool feed(uint64_t idx, const std::string& val)
//    {
//        return true;
//    }
//};
//
//
//struct FitFunc12Judger: FitFunc1Judger{
//    // a + b*x + c*x*x = (dy, ey)
//    double a;
//    double b;
//    double c;
//
//    double Calc(double x)
//    {
//        return a + b*x + c*x*x;
//    }
//
//    //bool match(double x, double y)
//    //{
//    //    return a + b*x + c*x*x;
//    //}
//
//    bool feed(uint64_t idx, const std::string& val)
//    {
//        return true;
//    }
//};
//
//template<class ItemType>
//struct FitFunc21Judger{
//
//};
//
//template<class ItemType>
//class FitFunc22Judger{
//
//};


NS_END_YME__

#endif // H_JGROUP__


