#include<iostream>
#include<map>
#include<list>
#include<vector>


class LRUCache{
    public:
        typedef std::pair<int, int>                  KvPackT;
        typedef std::list<KvPackT>::iterator       ListIterT;
        typedef std::map<int, ListIterT>::iterator  MapIterT;

    public:
        LRUCache(int fix_size=10000)
            : size_(fix_size)
        {
        }

    public:
        void set(int key, int val)
        {
            MapIterT mit = kmap_.find(key);
            if(mit != kmap_.end()){
                list_.erase(mit->second);
                kmap_.erase(mit);
            }
            if(kmap_.size() >= size_){
                int okey = list_.rbegin()->first;
                mit = kmap_.find(okey);
                list_.erase(mit->second);
                kmap_.erase(mit);
            }

            list_.push_front(KvPackT(key, val));
            kmap_[key] = list_.begin();
        }

        int get(int key)
        {
            int ret = -1;
            MapIterT it = kmap_.find(key);
            if(it != kmap_.end()){
                ret = it->second->second;
                set(key, it->second->second);
            }
            return ret;
        }

    private:
        int                      size_;
        std::list<KvPackT>       list_;
        std::map<int, ListIterT> kmap_;
};


int main(void)
{
    LRUCache cache(2);

    cache.set(2,1);
    cache.set(1,1);
    std::cout << cache.get(2) << std::endl;
    cache.set(4,1);
    std::cout << cache.get(1) << std::endl;
    std::cout << cache.get(2) << std::endl;
    return 0;
}

