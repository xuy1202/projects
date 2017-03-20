#ifndef H_BITS__
#define H_BITS__

#include <yme/helper.hpp>
#include <bitset>

#define NS_BEG_BITS__ namespace bits{
#define NS_END_BITS__ }


/*
 * must commit 2**_BITSMAP_STEP_BITS == _BITSMAP_STEP_SIZE
 */
#define _BITSMAP_STEP_SIZE 16
#define _BITSMAP_STEP_BITS 4 


NS_BEG_YME__
NS_BEG_BITS__

class BitsMap{
    public:
        BitsMap(): opens_(0) {
        }

    public:
        void shrink_fit()
        {
            auto it = map_.begin();
            while(it != map_.end()){
                if(it->second.none()){
                    map_.erase(it++);
                }else{
                    ++it;
                }
            }
        }

        struct Mapper{
            virtual void operator () (uint64_t idx) const
            {
                std::cout << "yme::bits::BitsMap::Mapper: " << idx << std::endl;
            }
        };

        inline void map(const Mapper* mapper) const
        {
            for(auto it=map_.begin(); it!=map_.end(); ++it){
                uint64_t ist = it->first << _BITSMAP_STEP_BITS;
                for(auto i=0; i<_BITSMAP_STEP_SIZE; ++i){
                    if(it->second.test(i)){
                        (*mapper)(ist + i);
                    }
                }
            }
        }

        bool     any()   const { return (opens_>0); }
        uint64_t opens() const { return     opens_; }

        std::string str() const
        {
            std::ostringstream oss;
            oss << "<BitsMap>\n";
            for(auto it=map_.begin(); it!=map_.end(); ++it){
                if(it->second.none()){
                    continue;
                }
                oss << "  " << it->second.to_string() << ": " << it->first << "\n";
            }
            return oss.str();
        }

    public:
        void open(uint64_t pos)
        {
            uint64_t idx = pos >> _BITSMAP_STEP_BITS;
            uint64_t rem = pos - ((pos >> _BITSMAP_STEP_BITS) << _BITSMAP_STEP_BITS);
            Unit& unit = map_[idx];
            if(unit.test(rem)){
                return;
            }
            unit.set(rem);
            ++opens_;
        }

        bool is_open(uint64_t pos) const
        {
            uint64_t idx = pos >> _BITSMAP_STEP_BITS;
            auto it = map_.find(idx);
            if(it == map_.end()){
                return false;
            }
            uint64_t rem = pos - ((pos >> _BITSMAP_STEP_BITS) << _BITSMAP_STEP_BITS);
            return it->second.test(rem);
        }

        void close(uint64_t pos)
        {
            uint64_t idx = pos >> _BITSMAP_STEP_BITS;
            uint64_t rem = pos - ((pos >> _BITSMAP_STEP_BITS) << _BITSMAP_STEP_BITS);
            Unit& unit = map_[idx];
            if(! unit.test(rem)){
                return;
            }
            unit.reset(rem);
            --opens_;
        }

        bool is_close(uint64_t pos) const
        {
            return (! is_open(pos));
        }

        void flip(uint64_t pos)
        {
            uint64_t idx = pos >> _BITSMAP_STEP_BITS;
            uint64_t rem = pos - ((pos >> _BITSMAP_STEP_BITS) << _BITSMAP_STEP_BITS);
            Unit& unit = map_[idx];
            if(unit.test(rem)){
                --opens_;
            }else{
                ++opens_;
            }
            unit.flip(rem);
        }
        
        void close_if_open(uint64_t pos)
        {
            uint64_t idx = pos >> _BITSMAP_STEP_BITS;
            auto it = map_.find(idx);
            if(it == map_.end()){
                return;
            }
            uint64_t rem = pos - ((pos >> _BITSMAP_STEP_BITS) << _BITSMAP_STEP_BITS);
            if(it->second.test(rem)){
                --opens_;
                it->second.reset(rem);
            }
        }

        void clear()
        {
            map_.clear();
        }


    public:
        //bool And(const BitsMap& rh)
        //{
        //    for(auto it=rh.map_.begin(); it!=rh.map_.end(); ++it){
        //        auto jt = map_.find(it->first);
        //        if(jt == map_.end()){
        //            continue;
        //        }
        //        jt->second &= it->second;
        //    }
        //    for(auto jt=map_.begin(); jt!=map_.end(); ++jt){
        //        auto it = rh.map_.find(jt->first);
        //        if(it != rh.map_.end()){
        //            continue;
        //        }
        //        jt->second.reset();
        //    }
        //}

        BitsMap Sub(const BitsMap& rh) const
        {
            BitsMap retm;
            for(auto it=map_.begin(); it!=map_.end(); ++it){
                uint64_t ist = it->first << _BITSMAP_STEP_BITS;
                for(auto i=0; i<_BITSMAP_STEP_SIZE; ++i){
                    uint64_t idx = ist + i;
                    if(! is_open(idx)){
                        continue;
                    }
                    if(rh.is_open(idx)){
                        continue;
                    }
                    retm.open(idx);
                }
            }
            return retm;
        }

        //bool Or(const BitsMap& rh)
        //{
        //    for(auto it=rh.map_.begin(); it!=rh.map_.end(); ++it){
        //        map_[it->first] |= it->second;
        //    }
        //}

    private:
        typedef std::bitset<_BITSMAP_STEP_SIZE> Unit;

        uint64_t               opens_;
        std::map<uint64_t, Unit> map_;
};


NS_END_BITS__
NS_END_YME__

#endif // H_BITS__


