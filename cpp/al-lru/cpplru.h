#ifndef CPPLRU_H__
#define CPPLRU_H__

#include<iostream>
#include<map>
#include<list>
#include<inttypes.h>

#define NS_BEG_CPPLRU__ namespace cpplru {
#define NS_END_CPPLRU__ }


NS_BEG_CPPLRU__

template<class KeyT, class ValT>
class LRUMap{
    public:
        typedef std::pair<KeyT, ValT>                 KvPackT;
        typedef typename std::list<KvPackT>::iterator ListIterT;

    public:
        LRUMap(uint32_t fix_size)
            : size_(fix_size), bal_lost_(0), get_miss_(0)
        {
        }

    public:
        void set(const KeyT& key, const ValT& val)
        {
            if(kmap_.size() >= size_){
                // if has the same key, just delete the old
                //     otherwise, we have to delete the oldest one
                if(has(key)){
                    typename std::map<KeyT, ListIterT>::iterator it;
                    it = kmap_.find(key);
                    if(it != kmap_.end()){
                        list_.erase(it->second);
                        kmap_.erase(it);
                    }
                }else{
                    const KeyT& okey = list_.rbegin()->first;
                    typename std::map<KeyT, ListIterT>::iterator it;
                    it = kmap_.find(okey);
                    if(it != kmap_.end()){
                        kmap_.erase(it);
                    }
                    list_.pop_back();
                }
                ++bal_lost_;
            }
            list_.push_front(KvPackT(key, val));
            kmap_[key] = list_.begin();
        }

        bool has(const KeyT& key)
        {
            ValT* p = get(key);
            if(p == NULL){
                return false;
            }
            return true;
        }

        ValT* get(const KeyT& key)
        {
            typename std::map<KeyT, ListIterT>::iterator it;
            it = kmap_.find(key);
            if(it == kmap_.end()){
                ++get_miss_;
                return NULL;
            }
            return &(it->second->second);
        }

        bool pop(const KeyT& key, ValT& val)
        {
            typename std::map<KeyT, ListIterT>::iterator it;
            it = kmap_.find(key);
            if(it == kmap_.end()){
                ++get_miss_;
                return false;
            }
            val = it->second->second;
            kmap_.erase(it);
            list_.erase(it->second);
            return true;
        }

    public:
        void show()
        {
            std::cout << "LRUMap (size: " << size_ << ")" << std::endl;
            for(ListIterT it=list_.begin(); it!=list_.end(); ++it){
                std::cout << "    " << it->first << " : " << it->second << std::endl;
            }
        }

        uint64_t miss_count() const { return get_miss_; }
        uint64_t lost_count() const { return bal_lost_; }

    private:
        uint32_t                      size_;
        uint64_t                  get_miss_;
        uint64_t                  bal_lost_;
        std::list<KvPackT>            list_;
        std::map<KeyT, ListIterT>     kmap_;
};


NS_END_CPPLRU__


#endif // CPPLRU_H__


