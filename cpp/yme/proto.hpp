#ifndef H_YME_PROTO__
#define H_YME_PROTO__

#include <yme/helper.hpp>


NS_BEG_YME__


struct DataItem{
    enum DataType{
        unknown = 0,
        kNumber = 1,
        kString = 2,
        kDouble = 3,
    };

    union UnionVal{
        int64_t       int64t_value;
        double        double_value;
        const std::string* str_ptr;
    };

    DataType type;
    UnionVal value;

    DataItem(): type(unknown){
    };

    bool operator < (const DataItem& rh) const
    {
        if(type <  rh.type){
            return true;
        }
        if(type == rh.type){
            switch(type){
                case kNumber:
                    return value.int64t_value < rh.value.int64t_value;
                case kDouble:
                    return (value.double_value - rh.value.double_value) < -0.000001;
                case kString:
                    return *(value.str_ptr)   < *(rh.value.str_ptr);
                default:
                    return false;
            }
        }
        return false;
    }

    bool operator == (const DataItem& rh) const
    {
        if(type != rh.type) return false;
        switch(type){
            case kNumber:
                return value.int64t_value == rh.value.int64t_value;
            case kDouble:
                return abs(value.double_value - rh.value.double_value) < 0.000001;
            case kString:
                return *(value.str_ptr)   == *(rh.value.str_ptr);
            default:
                return true;
        }
    }

    std::string str() const
    {
        std::ostringstream oss;
        switch(type){
            case kNumber: 
                oss << "<Number:" << value.int64t_value  << ">";
                break;
            case kString:
                oss << "<String:" << *value.str_ptr      << ">";
                break;
            case kDouble:
                oss << "<Double:" << value.double_value  << ">";
                break;
            default:
                oss << "<Unknown>";
        }
        return oss.str();
    }

    void show() const
    {
        std::cout << str() << std::endl;
    }
};


struct _OperProto{
    std::map<std::string, std::set<std::string> > tmap_;

    _OperProto()
    {
        tmap_["equal"].insert("string");
        tmap_["equal"].insert("number");
        tmap_["in_span"].insert("number");
        tmap_["in_span"].insert("double");
        //tmap_["fitfunc_11"].insert("double");
        //tmap_["fitfunc_12"].insert("double");
    }

    bool Support(const std::string& ope, const std::string& tpe)
    {
        auto it = tmap_.find(ope); 
        if(it == tmap_.end()){
            return false;
        }
        auto jt = it->second.find(tpe);
        if(jt == it->second.end()){
            return false;
        }
        return true;
    }
} OperProto;


struct DataProto{
    struct Conf{
        uint64_t match_order_priority;
        uint64_t   speedup_cache_size;
        uint64_t  speedup_cache_limit;

        Conf()
            : match_order_priority(0)
              , speedup_cache_size(0)
              , speedup_cache_limit(1)
        {
        }

        Conf(uint64_t p)
            : match_order_priority(p)
              , speedup_cache_size(0)
              , speedup_cache_limit(1)
        {
        }
    };

    std::map<std::string, std::string> tmap_;
    std::map<std::string, Conf*      > cmap_;

    bool Register(const std::string& key
                , DataItem::DataType type
                , Conf* conf=NULL)
    {
        if(tmap_.find(key) != tmap_.end()){
            return false;
        }
        switch(type){
            case DataItem::kNumber:
                tmap_[key] = "number";
                break;
            case DataItem::kString:
                tmap_[key] = "string";
                break;
            case DataItem::kDouble:
                tmap_[key] = "double";
                break;
            default:
                return false;
        }
        Conf* c = conf;
        if(c == NULL){
            c = new Conf();
        }
        cmap_[key] = c;
        return true;
    }

    uint64_t GetPriority(const std::string& key) const
    {
        auto it = cmap_.find(key);
        if(it == cmap_.end()){
            return 0;
        }
        return it->second->match_order_priority;
    }

    const std::string& GetType(const std::string& key) const
    {
        static const std::string s = "unknown";
        auto it = tmap_.find(key); 
        if(it == tmap_.end()){
            return s;
        }
        return it->second;
    }

    std::map<std::string, std::string>::const_iterator IterBeg() const
    {
        return tmap_.begin();
    }

    std::map<std::string, std::string>::const_iterator IterEnd() const
    {
        return tmap_.begin();
    }
};


template<class RawdataType>
struct DataObject{
    virtual bool Feed(const RawdataType& data) = 0;

    virtual DataItem Get(const std::string& key) = 0;

#ifdef _YME_STRING_INDEX
    virtual const std::string& GetIndex() { return ""; }
#else 
    virtual int64_t            GetIndex() { return 0;  }
#endif
};



NS_END_BITS__

#endif // H_YME_PROTO__


