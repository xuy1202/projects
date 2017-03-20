#ifndef H_YME_RULE__
#define H_YME_RULE__

#include <yme/helper.hpp>
#include <yme/proto.hpp>

#define _YME_RULE_ITEM_SPLITOR  " "
#define _YME_RULE_VALUE_SPLITOR ";"


NS_BEG_YME__


/* ======================================================================
 * Rule
 * ======================================================================
 */
class Rule{
    public:
        struct Condition{
            bool    reverse; // not using 
            std::string key;
            std::string ope;
            std::string val;

            Condition(const std::string& k
                        , const std::string& o
                        , const std::string& v)
                : reverse(false), key(k), ope(o), val(v){
                }

            Condition()
                : reverse(false), key(""), ope(""), val(""){
                }
        };

    public:
        /* ======================================================================
         * Virtual Functions
         *     ParseXml
         *     DumpsXml
         *     ParseIni
         *     DumpsIni
         *     MatchCallback
         * ======================================================================
         */

        /* like: 
         * <Rule id="8888" index="0" attr_key1="attr_val1">
         *     <cond value="sipv4 equal 0"/>
         *     <cond value="sport equal 90"/>
         *     <cond value="dport in_span 80-8080"/>
         * </Rule>
         */
        virtual bool ParseXml(const std::string& conf)
        {
            return false;
        }

        virtual bool DumpsXml(std::string& conf) const
        {
            std::ostringstream oss;
            oss << "<Rule id=\"" << id << "\" index=\"" << idx << "\"";
            for(auto it=attrs.begin(); it!=attrs.end(); ++it){
                oss << " \"" << it->first << "\"=\"" << it->second << "\"";
            }
            oss << ">\n";
            for(auto it=conds.begin(); it!=conds.end(); ++it){
                oss << "    <cond" 
                    << " key=\"" << it->second.key << "\""
                    << " ope=\"" << it->second.ope << "\""
                    << " val=\"" << it->second.val << "\""
                    << "/>\n";
            }
            oss << "</Rule>";
            conf = oss.str();
            return true;
        }

        /* like:
         * 8899:attr index 0
         * 8899:attr attr_key1 attr_val1
         * 8899:cond sipv4 equal 0
         * 8899:cond sport equal 90
         * 8899:cond dport in_span 80-8080
         */
        virtual bool ParseIni(const std::string& conf)
        {
            std::vector<std::string> linevec;
            split_to_vector(conf, "\n", linevec);
            for(auto it=linevec.begin(); it!=linevec.end(); ++it){
                const std::string& line = *it;
                if(line.empty()){
                    continue;
                }
                if(! ParseIniLine(line)){
                    return false;
                }
            }
            return true;
        }

        bool ParseIniLine(const std::string& line)
        {
            std::vector<std::string> itemvec;
            split_to_vector(line, _YME_RULE_ITEM_SPLITOR, itemvec);
            if(itemvec.size() < 3){
                return false;
            }
            if(itemvec[0] == "attr"){
                const std::string& key = itemvec[1];
                const std::string& val = itemvec[2];
                AddRuleAttr(key, val);
                if(key == "index"){
#ifdef _YME_STRING_INDEX
                    idx = val;
#else
                    idx = strtoll(val.c_str(), NULL, 10);
#endif
                }
                return true;
            }
            if(itemvec[0] == "cond"){
                if(itemvec.size() < 4){
                    return false;
                }
                const std::string& key = itemvec[1];
                const std::string& ope = itemvec[2];
                const std::string& val = itemvec[3];
                //const std::string& tpe = DataProto::GetType(key);
                //if(tpe.empty()){
                //    return false;
                //}
                //if(! OperProto.Support(ope, tpe)){
                //    return false;
                //}
                AddRuleCond(key, ope, val);
                return true;
            }
            return false;
        }

        virtual bool DumpsIni(std::string& conf) const
        {
            std::ostringstream oss;
            for(auto it=attrs.begin(); it!=attrs.end(); ++it){
                oss << id << ":attr " 
                    << it->first << _YME_RULE_ITEM_SPLITOR << it->second << "\n";
            }
            for(auto it=conds.begin(); it!=conds.end(); ++it){
                oss << id << ":cond" 
                    << _YME_RULE_ITEM_SPLITOR << it->second.key
                    << _YME_RULE_ITEM_SPLITOR << it->second.ope
                    << _YME_RULE_ITEM_SPLITOR << it->second.val
                    << "\n";
            }
            conf = oss.str();
            return true;
        }

        virtual void MatchCallback() const
        {
            ShowIni();
        }

    public:
        /* ======================================================================
         * Inner Functions
         *     constructor
         *     AddRuleAttr
         *     AddRuleDesc
         *     ShowXml
         *     ShowIni
         * ======================================================================
         */
#ifdef _YME_STRING_INDEX
        Rule(uint64_t i, const std::string g=""): id(i), idx(g){}
#else
        Rule(uint64_t i, int64_t g=0): id(i), idx(g){}
#endif

        inline void AddRuleAttr(const std::string& k, const std::string& v)
        {
            attrs[k] = v;
        }

        inline std::string GetRuleAttr(const std::string& k) const
        {
           auto rt = attrs.find(k);
           if(rt == attrs.end()){
               return "";
           }
           return rt->second;
        }

        inline void AddRuleCond(const std::string& k
                    , const std::string& o
                    , const std::string& v)
        {
            conds[k] = Condition(k, o, v);
        }

        inline void ShowXml() const
        {
            std::string conf;
            if(DumpsXml(conf)){
                std::cout << conf << std::endl;
            }
        }

        inline void ShowIni() const
        {
            std::string conf;
            if(DumpsIni(conf)){
                std::cout << conf << std::endl;
            }
        }

        inline uint64_t    Id() const { return id;  }

#ifdef _YME_STRING_INDEX
        const std::string& Index() const { return idx; }
#else
        inline int64_t     Index() const { return idx; }
#endif

        inline std::map<std::string, std::string>::const_iterator AttrIterBeg() const
        {
            return attrs.begin();
        }

        inline std::map<std::string, std::string>::const_iterator AttrIterEnd() const
        {
            return attrs.end();
        }

        inline std::map<std::string, Condition>::const_iterator CondIterBeg() const
        {
            return conds.begin();
        }

        inline std::map<std::string, Condition>::const_iterator CondIterEnd() const
        {
            return conds.end();
        }

        const Condition* GetCond(const std::string& key) const
        {
            auto it = conds.find(key);
            if(it == conds.end()){
                return NULL;
            }
            return &(it->second);
        }

    private:
        uint64_t id;
#ifdef _YME_STRING_INDEX
        const std::string& idx;
#else
        int64_t            idx;
#endif
        std::map<std::string, std::string> attrs;
        std::map<std::string, Condition>   conds;
};


template<class RawType, class DatType, class RuleType, class RetType>
class RuleFormatter{
    private:
        std::map<uint64_t, RuleType*>                   rmap_;
        const Engine<RawType, DatType, RuleType, RetType>* e_;

    public:
        RuleFormatter(const Engine<RawType, DatType, RuleType, RetType>* e)
            : e_(e){
            }

        bool LoadFromFile(const std::string& path)
        {
            std::ifstream infile;                                                                                                                                                  
            infile.open(path.c_str(), std::ifstream::in);
            if(! infile.is_open()){
                e_->GetLogger()->Error("RuleFormatter OpenFile Error: " + path);
                return false;
            }
            std::string line;
            while(infile.good()){
                getline(infile, line, '\n');
                // strip
                line.erase(0, line.find_first_not_of(" \t"));
                line.erase(line.find_last_not_of(" \t") + 1);
                // empty line
                if(line.empty()){
                    continue;
                }
                // line should like "id:cond key ope val"
                std::vector<std::string> vec;
                split_to_vector(line, ":", vec);
                if(vec.size() != 2){
                    e_->GetLogger()->Error("RuleFormatter Line Error: " + line);
                    return false;
                }
                if(vec[0].empty() || vec[0][0] == '#' || vec[0][0] == ';'){
                    continue;
                }
                uint64_t id = static_cast<uint64_t>(strtoll(vec[0].c_str(), NULL, 10));
                if(rmap_.find(id) == rmap_.end()){
                    rmap_[id] = new RuleType(id);
                }
                if(! rmap_[id]->ParseIniLine(vec[1])){
                    e_->GetLogger()->Error("RuleFormatter Parse Error: " + line);
                    return false;
                }
            }
            return true;
        }

        std::map<uint64_t, Rule*>::iterator RuleIterBeg()
        {
            return rmap_.begin();
        }

        std::map<uint64_t, Rule*>::iterator RuleIterEnd()
        {
            return rmap_.end();
        }
};


/* ======================================================================
 * MatchedResult
 * ======================================================================
 */

template<class DatType>
struct MatchedResult{
    enum ResultType{
        unknown    = 0,
        kParseFail = 1,
        kIndexFail = 2,

        kMatchNone = 8,
        kMatchSucc = 9,
    };
    ResultType                 MatchedCode;
    std::map<uint64_t, Rule*>  MatchedRule;

    std::map<std::string, std::string> Ext;

    DatType                           Data;

    MatchedResult()
        : MatchedCode(unknown)
    {
    }

    virtual void MatchedCallback() const
    {
        for(auto it=MatchedRule.begin(); it!=MatchedRule.end(); ++it){
            std::cout << ">>> Matched " << it->first << std::endl;
            assert(it->second != NULL);
            it->second->MatchCallback();
        }
    }

    virtual void NoMatchCallback() const
    {
        std::cout << ">>> Matched None" << std::endl;
    }
};


NS_END_YME__


#endif // H_YME_RULE__


