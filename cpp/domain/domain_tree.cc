
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <inttypes.h>
#include <fstream>


void split_to_vector(const std::string& in
            , const std::string& sep 
            , std::vector<std::string>& v)
{
    std::string::size_type start_pos = 0;
    std::string::size_type find_pos  = std::string::npos;
    do{ 
        find_pos = in.find(sep, start_pos);
        if (find_pos == std::string::npos){
            break;
        }   
        if (find_pos > in.size()){
            break;
        }   
        std::string item = in.substr(start_pos, find_pos-start_pos);
        v.push_back(item);
        start_pos = find_pos + sep.size();
    }while(1);
    std::string final_item = in.substr(start_pos, in.size()-start_pos);
    v.push_back(final_item);
}


static struct TagMap{
    std::vector<std::string>        idx_to_tag_vec;
    std::map<std::string, uint64_t> tag_to_idx_map;

    TagMap()
    {
        get_set_idx("<ROOT>");
    }

    uint64_t get_idx(const std::string& tag)
    {
        static uint64_t max = 0xffffffffffffffff;
        auto i = tag_to_idx_map.find(tag);
        if(i == tag_to_idx_map.end()){
            return max;
        }
        return i->second;
    }

    uint64_t get_set_idx(const std::string& tag)
    {
        auto i = tag_to_idx_map.find(tag);
        if(i == tag_to_idx_map.end()){
            uint64_t cur_size = tag_to_idx_map.size();
            tag_to_idx_map[tag] = cur_size;
            idx_to_tag_vec.push_back(tag);
            if(cur_size % 100000 == 0){
                std::cout << "cur_size: " << tag << " " << cur_size << std::endl;
            }
            return cur_size;
        }
        return i->second;
    }

    std::string get_tag(uint64_t idx) const
    {
        if(idx >= idx_to_tag_vec.size()){
            return "";
        }
        return idx_to_tag_vec[idx];
    }
} GlobalTagSet;


struct DomainNode{
    static uint64_t stat_nums_;

    uint64_t                        tagidx;
    DomainNode*                     father;
    std::map<uint64_t, DomainNode*>*  sons;

    DomainNode()
        : tagidx(GlobalTagSet.get_set_idx("<ROOT>")), father(NULL), sons(NULL)
    {
        ++stat_nums_;
    }

    DomainNode(const std::string& k)
        : tagidx(GlobalTagSet.get_set_idx(k)), father(NULL), sons(NULL)
    {
        ++stat_nums_;
    }

    DomainNode(const std::string& k, DomainNode* f)
        : tagidx(GlobalTagSet.get_set_idx(k)), father(f), sons(NULL)
    {
        ++stat_nums_;
    }

    bool has(const std::string& k)
    {
        if(sons){
            return (sons->find(GlobalTagSet.get_idx(k)) != sons->end());
        }
        return false;
    }

    DomainNode* get(const std::string& k)
    {
        if(sons == NULL){
            sons = new std::map<uint64_t, DomainNode*>();
        }
        auto it = sons->find(GlobalTagSet.get_set_idx(k));
        if(it == sons->end()){
            DomainNode* n = new DomainNode(k, this);
            (*sons)[GlobalTagSet.get_set_idx(k)] = n;
            return n;
        }
        return it->second;
    }

    void show() const
    {
        std::cout << "<" << GlobalTagSet.get_tag(tagidx) << ">";
        if(father){
            std::cout << "<" << GlobalTagSet.get_tag(father->tagidx) << ">";
        }
        std::cout << "[";
        if(sons){
            for(auto it=sons->begin(); it!=sons->end(); ++it){
                std::cout << GlobalTagSet.get_tag(it->second->tagidx) << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
};


uint64_t DomainNode::stat_nums_ = 0;


class DomainTree{
    public:
        DomainTree(): root(), reverse(false)
    {
    }

    public:
        void feed(const std::string& domain_name)
        {
            std::vector<std::string> vec;
            split_to_vector(domain_name, ".", vec);

            //std::cout << 1 << ": " << domain_name << std::endl;
            DomainNode* curnode = &root;
            if(reverse){
                for(auto it=vec.rbegin(); it!=vec.rend(); ++it){
                    curnode = curnode->get(*it);
                }
            }else{
                for(auto it=vec.begin(); it!= vec.end(); ++it){
                    curnode = curnode->get(*it);
                    //std::cout << 2 << ": " << *it << std::endl;
                }
            }
        }

        bool load(const std::string& file_path)
        {
            std::ifstream infile;
            infile.open(file_path.c_str(), std::ifstream::in);
            if(! infile.is_open()){
                return false;
            }
            std::string line;
            uint64_t c = 0;
            while(infile.good()){
                getline(infile, line, '\n');
                feed(line);
                ++c;
                if(c% 100000 == 0){
                    std::cout << c << " " << line << std::endl;
                }
            }
            return true;
        }

        bool have(const std::string& domain_name)
        {
            DomainNode* curnode = find_node_by_path(domain_name);
            return (curnode != NULL);
        }

        bool seek_do(const std::string& domain_prefix)
        {
            DomainNode* curnode = find_node_by_path(domain_prefix);
            if(curnode == NULL) return false;

            curnode->show();
            return true;
        }

        struct WalkStat{
            const DomainNode*      curnode;
            std::vector<std::string> nodes;
            bool                   leafend;
            bool                 need_stop;
        };
        struct Walker {
            virtual void operator () (WalkStat& stat) = 0;
        };

        void walk_from_prefix(Walker* walker, const std::string& prefix="")
        {
            DomainNode* curnode = find_node_by_path(prefix);
            if(curnode == NULL) {
                return;
            }
            walk_from_node(walker, curnode);
        }

        void walk_from_node(Walker* walker
                    , const DomainNode* node
                    , WalkStat* stat=NULL)
        {
            WalkStat* curstat = NULL;
            if(stat == NULL){
                curstat = new WalkStat;
            }else{
                curstat = stat;
            }
            if(curstat->need_stop){
                return;
            }
            curstat->curnode = node;
            curstat->nodes.push_back(GlobalTagSet.get_tag(node->tagidx));
            if(node->sons == NULL || node->sons->size() == 0){
                curstat->leafend = true;
            }else{
                curstat->leafend = false;
            }

            (*walker)(*curstat);
            if(curstat->need_stop){
                return;
            }

            if(node->sons){
                for(auto i=node->sons->begin(); i!=node->sons->end(); ++i){
                    walk_from_node(walker, i->second, curstat);
                }
            }
            curstat->nodes.pop_back();
        }

    private: 
        DomainNode* find_node_by_path(const std::string& domain_name)
        {
            if(domain_name.empty()){
                return &root;
            }

            std::vector<std::string> vec;
            split_to_vector(domain_name, ".", vec);

            DomainNode* curnode = &root;
            if(reverse){
                for(auto it=vec.rbegin(); it!=vec.rend(); ++it){
                    if(! curnode->has(*it)){
                        return NULL;
                    }
                    curnode = curnode->get(*it);
                }
            }else{
                for(auto it=vec.begin(); it!=vec.end(); ++it){
                    if(! curnode->has(*it)){
                        return NULL;
                    }
                    curnode = curnode->get(*it);
                }
            }
            return curnode;
        }

    public:
        DomainNode root;
        bool    reverse;
};


struct LevelWalkFilter: public DomainTree::Walker {
    uint32_t max_print;
    uint32_t min_level;
    uint32_t    _count;

    virtual void operator () (DomainTree::WalkStat& stat)
    {
        if(stat.nodes.size() < min_level){
            return;
        }
        for(auto i=stat.nodes.begin(); i!=stat.nodes.end(); ++i){
            std::cout << "." << *i;
        }
        std::cout << " " << stat.leafend << std::endl;
        ++_count;
        if(_count >= max_print){
            stat.need_stop = true;
        }
    }
};


int main(int argc, char* argv[])
{
    if(argc < 2){
        std::cout << "load from fucking where?" << std::endl;
        exit(-1);
    }
    std::string file_path(argv[1]);
    std::cout << "Load From: " << file_path << std::endl;

    DomainTree tree;
    std::cout << tree.load(file_path.c_str()) << std::endl;
    std::cout << DomainNode::stat_nums_ << std::endl;

    tree.seek_do("ac.120v");

    LevelWalkFilter* walker = new LevelWalkFilter();
    walker->max_print = 10;
    walker->min_level = 5;
    tree.walk_from_prefix(walker);

    //tree.seek_do("org.zzlg");
    //tree.walk_from_prefix(cb, "org.zzlg");

    while(true) sleep(60);

    //tree.feed("a.b.c.d");
    //tree.feed("aa.bb.c.d");
    //std::cout << tree.have("a.b.c.d") << std::endl;
    //std::cout << tree.have("a.b.c.d.e") << std::endl;
    //std::cout << tree.have("a.b.c.dd") << std::endl;

    //DomainNode n;
    //std::cout << sizeof(n) << std::endl;

    return 0;
}


