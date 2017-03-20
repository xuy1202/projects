
#include<iostream>
#include<map>
#include<vector>
#include<bitset>
#include<inttypes.h>
#include<string.h>


void show_char(char chr)
{
    std::cout << ((chr & (0x1 << 0)) >> 0);
    std::cout << ((chr & (0x1 << 1)) >> 1);
    std::cout << ((chr & (0x1 << 2)) >> 2);
    std::cout << ((chr & (0x1 << 3)) >> 3);
    std::cout << ((chr & (0x1 << 4)) >> 4);
    std::cout << ((chr & (0x1 << 5)) >> 5);
    std::cout << ((chr & (0x1 << 6)) >> 6);
    std::cout << ((chr & (0x1 << 7)) >> 7);
    std::cout << std::endl;
}


void show_str(const std::string& out)
{
    std::cout << ">>> " << out << std::endl;
    for(auto it = out.begin(); it!=out.end(); ++it){
        show_char(*it);
    }
}


class HuffmanTree{
    public:
        struct Node{
            uint64_t w;
            char     c;
            char     o;
            Node*   n0;
            Node*   n1;
            Node*   nf;

            Node()
                : w(0), c(0x0), o(0x0), n0(NULL), n1(NULL), nf(NULL)
            {
            }
        };

        struct NodeReader{
            const Node*  root;
            const Node*  node;
            std::string* rets;
            NodeReader(const Node* n, std::string& s)
                : root(n), node(n), rets(&s)
            {
                rets->clear();
            }

            void feed_char(const char chr)
            {
                if(chr == '0'){
                    node = node->n0;
                }else{
                    node = node->n1;
                }
                if(node->o != 0x0){
                    rets->append(1, node->o);
                    node = root;
                }
            }

            void feed_uint(unsigned chr)
            {
                if(chr == 0){
                    node = node->n0;
                }else{
                    node = node->n1;
                }
                if(node->o != 0x0){
                    rets->append(1, node->o);
                    node = root;
                }
            }
        };

    private:
        std::map<char, uint64_t>  cmap_;
        std::map<char, Node*>     emap_;
        std::map<uint64_t, Node*> rmap_;
        Node*                     root_;
        std::map<uint64_t, Node*> tree_; 
        std::vector<std::string>  mmap_;

    public:
        void show()
        {
            for(auto i=0; i<mmap_.size(); ++i){
                if(mmap_[i].empty()){
                    continue;
                }
                std::cout << static_cast<char>(i) << " " << mmap_[i] << std::endl;
            }
        }

        void feed(char chr, uint64_t count=1)
        {
            cmap_[chr] += count;
        }

        void format_rmap()
        {
            for(auto it=cmap_.begin(); it!=cmap_.end(); ++it){
                char     chr = it->first;
                uint64_t cnt = it->second;
                while(rmap_.find(cnt) != rmap_.end()){
                    ++cnt;
                }
                Node* node = new Node();
                node->o = chr;
                node->w = cnt;
                rmap_[cnt] = node;
                emap_[chr] = node;
            }
            cmap_.clear();
        }

        void build()
        {
            feed(0x0, 0); // sentry for ending
            format_rmap();
            while(rmap_.size() > 1){
                uint64_t lcnt = rmap_.begin()->first;
                Node*    lnod = rmap_.begin()->second;
                rmap_.erase(rmap_.begin());
                uint64_t rcnt = rmap_.begin()->first;
                Node*    rnod = rmap_.begin()->second;
                rmap_.erase(rmap_.begin());

                uint64_t tcnt = lcnt + rcnt;

                Node* node = new Node();
                node->w = tcnt;
                node->n0 = lnod;
                node->n1 = rnod;
                lnod->nf = node;
                rnod->nf = node;
                lnod->c = '0';
                rnod->c = '1';

                while(rmap_.find(tcnt) != rmap_.end()){
                    ++tcnt;
                }

                rmap_[tcnt] = node;
            }
            root_ = rmap_.begin()->second;
            format_bits();
        }

        bool format_bits()
        {
            while(mmap_.size() < 255){
                mmap_.push_back("");
            }
            for(auto it=emap_.begin(); it!=emap_.end(); ++it){
                const Node* node = it->second;
                char buf[32] = {0};
                int  idx = 0;
                while(node->nf){
                    buf[idx++] = node->c;
                    node = node->nf;
                }
                int mid = idx >> 1;
                for(int i=0; i<mid; ++i){
                    char* lh = buf + i;
                    char* rh = buf + idx - 1 - i;
                    *lh = (*lh) ^ (*rh);
                    *rh = (*lh) ^ (*rh);
                    *lh = (*lh) ^ (*rh);
                }
                mmap_[it->first] = buf;
            }
        }

        int32_t compress(const std::string& in, std::string& out) const
        {
            out.clear();
            char    chr = 0;
            int32_t idx = 0;
            int32_t cnt = 0;
            for(auto it=in.begin(); it!=in.end(); ++it){
                const std::string& chrs = mmap_[*it];
                if(chrs.empty()){
                    return -1;
                }
                cnt += chrs.size();
                for(auto jt=chrs.begin(); jt!=chrs.end(); ++jt){
                    if(*jt == '1') chr |= (0x1 << idx);
                    ++idx;
                    if(idx == 8){
                        out.append(1, chr);
                        idx = 0;
                        chr = 0;
                    }
                }
            }
            if(idx != 0){
                // using 0x0 pattern to fill the rest
                const std::string& chrs = mmap_[0x0];
                auto jt = chrs.begin();
                while(idx < 8){
                    if(*jt == '1'){
                        chr |= (0x1 << idx);
                    }
                    if(++jt == chrs.end()) jt = chrs.begin();
                    ++idx;
                }
                out.append(1, chr);
            }
            return cnt;
        }

        bool decompress(const std::string& in, std::string& out)
        {
            NodeReader reader(root_, out);
            for(auto it=in.begin(); it!=in.end(); ++it){
                unsigned char chr = *it;
                reader.feed_uint((chr & (0x1 << 0)) >> 0);
                reader.feed_uint((chr & (0x1 << 1)) >> 1);
                reader.feed_uint((chr & (0x1 << 2)) >> 2);
                reader.feed_uint((chr & (0x1 << 3)) >> 3);
                reader.feed_uint((chr & (0x1 << 4)) >> 4);
                reader.feed_uint((chr & (0x1 << 5)) >> 5);
                reader.feed_uint((chr & (0x1 << 6)) >> 6);
                reader.feed_uint((chr & (0x1 << 7)) >> 7);
            }
            // for 0x0 pattern ending
            while(*out.rbegin() == 0x0) out.resize(out.size()-1);
            return true;
        }
};


int main(void)
{
    HuffmanTree tree;
    tree.feed('.', 868252570); 
    tree.feed('o', 515807598); 
    tree.feed('c', 463167785); 
    tree.feed('a', 431828228); 
    tree.feed('e', 350413724); 
    tree.feed('n', 344923530); 
    tree.feed('m', 343333368); 
    tree.feed('i', 313362109); 
    tree.feed('1', 308560589); 
    tree.feed('0', 264322038); 
    tree.feed('s', 229462199); 
    tree.feed('2', 213225567); 
    tree.feed('w', 202549037); 
    tree.feed('3', 198975153); 
    tree.feed('b', 197980100); 
    tree.feed('6', 197950085); 
    tree.feed('4', 197593388); 
    tree.feed('t', 196771508); 
    tree.feed('d', 188579792); 
    tree.feed('8', 176161761); 
    tree.feed('5', 175828138); 
    tree.feed('7', 171776126); 
    tree.feed('u', 171010515); 
    tree.feed('l', 167782611); 
    tree.feed('g', 165617889); 
    tree.feed('9', 159686019); 
    tree.feed('r', 157894187); 
    tree.feed('h', 154527060); 
    tree.feed('p', 121575528); 
    tree.feed('f', 121059022); 
    tree.feed('y', 101358237); 
    tree.feed('j', 82108655); 
    tree.feed('z', 80244625); 
    tree.feed('k', 75127875); 
    tree.feed('x', 73817571); 
    tree.feed('q', 67180863); 
    tree.feed('-', 62745607); 
    tree.feed('v', 58502466); 

    tree.build();
    //tree.show();

    std::vector<std::string> strs = {
        "360",
        "baidu",
        "cn",
        "com",
        "eu.doudouguo.sdf.0018033349",
        "com.baidu.galileo.ime.00b15fde43a70b52088b686bacac814eb58abd8b",
        "00b15fde43a70b52088b686bacac814eb58abd8b"
    };
    for(auto it=strs.begin(); it!=strs.end(); ++it){
        std::string o = *it;
        std::string c;
        std::string d;
        std::cout << std::endl;
        std::cout << "O: " << o.size() << " " << o << std::endl;
        tree.compress(o, c);
        std::cout << "C: " << c.size() << " " << c << std::endl;
        tree.decompress(c, d);
        std::cout << "D: " << d.size() << " " << d << std::endl;
    }
    std::string s= "com.baidu.galileo.ime.00b15fde43a70b52088b686bacac814eb58abd8b";
    std::string p;
    for(auto i=0; i<100000; ++i){
        tree.compress(s, p);
        tree.decompress(p, s);
    }
    return 0;
}




