#ifndef H_YME_ENGINE__
#define H_YME_ENGINE__

#include <yme/judger.hpp>
#include <yme/rule.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>


NS_BEG_YME__


template<class RawType, class DatType, class RuleType, class RetType>
class JudgeGroup{
    private:
        struct Judkey{
            uint64_t    pro; 
            std::string key;
            std::string ope;

            Judkey(uint64_t p, const std::string& k, const std::string& o)
                : pro(p), key(k), ope(o){
            }

            bool operator < (const Judkey& rh) const
            {
                if(pro > rh.pro){
                    return true;
                }
                if(pro == rh.pro
                            && key < rh.key){
                    return true;
                }
                if(pro == rh.pro
                            && key == rh.key
                            && ope < rh.ope){
                    return true;
                }
                return false;
            }

            bool operator == (const Judkey& rh) const
            {
                return (pro == rh.pro
                            && key == rh.key 
                            && ope == rh.ope
                       );
            }

            std::string str() const
            {
                char buf[256] = {0};
                auto len = snprintf(buf, 256, "%lu-%s-%s", pro, key.c_str(), ope.c_str());
                return std::string(buf, len);
            }
        };

    private:
        std::string                            gtag_;
        bool                            feed_closed_;
        bits::BitsMap                       inibits_;
        std::map<Judkey, Judger*>           judgers_;
        std::map<Judkey, bits::BitsMap* >   bmomits_;
        const Engine<RawType, DatType, RuleType, RetType>* e_;

    public:
#ifdef _YME_STRING_INDEX
        JudgeGroup(const std::string tag
                    , const Engine<RawType, DatType, RuleType, RetType>* e)
            : gtag_(tag), feed_closed_(false), e_(e){
            }
#else
        JudgeGroup(int64_t tag
                    , const Engine<RawType, DatType, RuleType, RetType>* e)
            : feed_closed_(false), e_(e){
                char buf[64] = {0};
                auto len = snprintf(buf, 64, "%ld", tag);
                gtag_ = std::string(buf, len);
            }
#endif

    public:
        bool FeedRule(Rule* rule)
        {
            if(feed_closed_){
                return false;
            }

            uint64_t rid = rule->Id();

            for(auto it=rule->CondIterBeg(); it!=rule->CondIterEnd(); ++it){
                const std::string& key = it->second.key;
                const std::string& ope = it->second.ope;
                const std::string& val = it->second.val;

                uint64_t pro = e_->GetDataProto()->GetPriority(key);
                Judkey jkey(pro, key, ope);

                if(judgers_.find(jkey) == judgers_.end()){
                    const std::string& tpe = e_->GetDataProto()->GetType(key);
                    std::string tag = tpe + "-" + ope;
                    if(tag == "string-equal"){
                        judgers_[jkey] = new StringItemJudger();
                    }else if(tag == "number-equal"){
                        judgers_[jkey] = new NumberItemJudger();
                    }else if(tag == "number-in_span"){
                        judgers_[jkey] = new NumberSpanJudger();
                    }else if(tag == "double-in_span"){
                        judgers_[jkey] = new DoubleSpanJudger();
                    }else{
                        e_->GetLogger()->Error("JudgeGroup Unsupported Method: " + key + " " + tag);
                        return false;
                    }
                    e_->GetLogger()->Info("JudgeGroup<" + gtag_ + "> Add Judger<" + jkey.str() + ">");
                    //if(tag == "double-fitfunc_11") judger = new FitFunc11Judger();
                    //if(tag == "double-fitfunc_12") judger = new FitFunc12Judger();
                }

                std::vector<std::string> vec;
                split_to_vector(val, _YME_RULE_VALUE_SPLITOR, vec);
                for(auto vt=vec.begin(); vt!=vec.end(); ++vt){
                    if(judgers_[jkey]->Feed(rid, *vt)){
                        continue;
                    }
                    e_->GetLogger()->Error("Judge <" + jkey.str() + "> Feed Error: " + *vt);
                    return false;
                }
            }

            inibits_.open(rid);
            return true;
        }

        void CloseFeed()
        {
            //e_->GetLogger()->Debug("JudgeGroup Init Rules: " 
            //            + inibits_.str()
            //            );
            for(auto it=judgers_.begin(); it!=judgers_.end(); ++it){
                bits::BitsMap* omit = new bits::BitsMap(inibits_.Sub(it->second->rids));
                if(omit->any()){
                    bmomits_[it->first] = omit;
                    judgers_[it->first]->RegisterOmits(omit);
                    e_->GetLogger()->Debug("JudgeGroup Omits: " 
                                + it->first.str() 
                                + "\n" 
                                + bmomits_[it->first]->str()
                                );
                }
            }
            std::ostringstream oss;
            oss << "JudgeGroup<" << gtag_ << "> Match Order: ";
            for(auto it=judgers_.begin(); it!=judgers_.end(); ++it){
                if(it != judgers_.begin()){
                    oss << " -> ";
                }
                oss << it->first.str();
            }
            e_->GetLogger()->Info(oss.str());
            feed_closed_ = true;
        }

        struct AddIdMapper: bits::BitsMap::Mapper{
            const std::map<uint64_t, Rule*>*  g_rule_map;
            std::map<uint64_t, Rule*>*       MatchedRule;

            void operator () (uint64_t idx) const
            {
                // rule may be poped out
                auto it = g_rule_map->find(idx);
                if(it != g_rule_map->end()){
                    (*MatchedRule)[idx] = it->second;
                }
            }
        };

        bool Match(DatType& data, RetType& result) const
        {
            bits::BitsMap retm;
            for(auto it=judgers_.begin(); it!=judgers_.end(); ++it){
                const std::string& key = it->first.key;
                DataItem item = data.Get(key);
#ifdef _YME_DEBUG
                if(e_->GetFullDebug()){
                    e_->GetLogger()->Debug("JudgeGroup::Match " + key + " = " + item.str());
                }
#endif
                if(item.type == DataItem::unknown){
                    continue;
                }
                Judger* judger = it->second;

                if(retm.opens() == 0){
                    retm = judger->Match(item, NULL);
                    //retm = judger->Match(item, &inibits_);
                }else{
                    retm = judger->Match(item, &retm);
                }

                if(! retm.any()){
                    result.MatchedCode = RetType::kMatchNone;
                    result.Ext["last_key"] = key;
                    return true;
                }
            }
            retm.shrink_fit();

            AddIdMapper mapper;
            mapper.MatchedRule = &(result.MatchedRule);
            mapper.g_rule_map  = e_->GetRuleMap();
            retm.map(&mapper);

            if(result.MatchedRule.empty()){
                result.MatchedCode = RetType::kMatchNone;
            }else{
                result.MatchedCode = RetType::kMatchSucc;
            }
            return true;
        }
};


template<class RawType, class DatType, class RuleType=Rule, class RetType=MatchedResult<DatType> >
class Engine{
    private:
        // Rule Related
        typedef JudgeGroup<RawType, DatType, RuleType, RetType> GroupJudgerType;
#ifdef _YME_STRING_INDEX
        std::map< std::string, GroupJudgerType* >         imap_;
#else
        std::map<     int64_t, GroupJudgerType* >         imap_;
#endif
        RuleFormatter<RawType, DatType, RuleType, RetType>*  rformatter_;

        std::string                 name_;
        DataProto                  proto_;
        std::map<uint64_t, Rule*>   rmap_;
        Logger                    logger_; 

        bool                  full_debug_;

        struct RuleStat{
            uint64_t rule_nums;
            uint64_t rule_pops;
            uint64_t rule_adds;
        } rule_stat_;

    public:
        Engine(const std::string n): rformatter_(NULL), name_(n)
        {
        }

        inline const std::map<uint64_t, Rule*>* GetRuleMap()   const { return &rmap_;   }
        inline const DataProto*                 GetDataProto() const { return &proto_;  }
        inline const Logger*                    GetLogger()    const { return &logger_; }

        inline const std::string& Name() const { return name_; }

        inline void SetFullDebug(bool b) { full_debug_ = b;    }
        inline bool GetFullDebug() const { return full_debug_; }

    public:
        bool Match(const RawType& rdata)
        {
            RetType result;
            if(! Match(rdata, result)){
                return false;
            }
            if(result.MatchedCode == RetType::kMatchSucc){
                result.MatchedCallback();
            }
            if(result.MatchedCode == RetType::kMatchNone){
                result.NoMatchCallback();
            }
            return true;
        }

        bool Match(const RawType& rdata, RetType& result)
        {
            if(! result.Data.Feed(rdata)){
#ifdef _YME_DEBUG
                if(GetFullDebug()){
                    GetLogger()->Debug(name_ + " Match::ParseFail");
                }
#endif
                result.MatchedCode = RetType::kParseFail;
                return false;
            }
            return Match(result);
        }

        bool Match(RetType& result)
        {
#ifdef _YME_STRING_INDEX
            const std::string& index = result.Data.GetIndex();
#else
            int64_t            index = result.Data.GetIndex();
#endif

            auto git = imap_.find(index);
            if(git == imap_.end()){
#ifdef _YME_DEBUG
                if(GetFullDebug()){
                    GetLogger()->Debug(name_ + " Match::IndexFail");
                }
#endif
                result.MatchedCode = RetType::kIndexFail;
                return false;
            }
            return git->second->Match(result.Data, result);
        }

        bool FeedRule(RuleType* rule)
        {
#ifdef _YME_STRING_INDEX
            const std::string& index = rule->Index();
#else
            int64_t            index = rule->Index();
#endif
            JudgeGroup<RawType, DatType, RuleType, RetType>* j = NULL;
            auto it = imap_.find(index);
            if(it == imap_.end()){
                j = new JudgeGroup<RawType, DatType, RuleType, RetType>(index, this);
                imap_[index] = j;
            }else{
                j = it->second;
            }
            if(j->FeedRule(rule)){
                rmap_[rule->Id()] = rule;
                return true;
            }
            return false;
        }

        void CloseFeed()
        {
            for(auto it=imap_.begin(); it!=imap_.end(); ++it){
                it->second->CloseFeed();
            }
        }

        bool Rebuild()
        {
            return true;
        }

        bool GetRule(uint64_t rid, std::string& rstr)
        {
            auto it = rmap_.find(rid);
            if(it == rmap_.end()){
                return false;
            }
            return it->second->DumpsIni(rstr);
        }

        bool PopRule(uint64_t rid)
        {
            return true;
        }

        bool SetRule(uint64_t, const std::string& rstr)
        {
            return true;
        }

        bool RegisterProto(const std::string& key
                    , DataItem::DataType type
                    , DataProto::Conf* conf=NULL
                    )
        {
            return proto_.Register(key, type, conf); 
        }

        RuleFormatter<RawType, DatType, RuleType, RetType>* GetRuleFormatter()
        {
            if(rformatter_ == NULL){
                rformatter_ = new RuleFormatter<RawType, DatType, RuleType, RetType>(this);
            }
            return rformatter_;
        }

        bool LoadFromRegFile(const std::string& cfg)
        {
            auto f = GetRuleFormatter();
            if(! f->LoadFromFile(cfg)){
                return false;
            }
            for(auto it=f->RuleIterBeg(); it!=f->RuleIterEnd(); ++it){
                //uint64_t id = it->first;
                yme::Rule* rule = it->second;
                if(! FeedRule(rule)){
                    return false;
                }
            }
            GetLogger()->Info(name_ + " Engine::LoadFromFile Succ: " + cfg);
            return true;
        }

        bool LoadFromDirFile(const std::string& dir)
        {
            struct dirent* ent = NULL;
            DIR *pDir = opendir(dir.c_str());
            if(pDir == NULL){
                GetLogger()->Error(name_ + " Engine::LoadFromFile OpenDir Fail: " + dir);
                return false;
            }
            while((ent=readdir(pDir)) != NULL){
                if(ent->d_type != DT_REG){
                    continue;
                }
                std::string fnm(ent->d_name);
                std::string cfg = (*dir.rbegin() == '/')? dir+fnm: dir+"/"+fnm;
                if(! LoadFromRegFile(cfg)){
                    GetLogger()->Error(name_ + " Engine::LoadFromFile Fail: " + cfg);
                    return false;
                }
            }
            return true;
        }

        bool LoadFromFile(const std::string& cfg)
        {
            struct stat info;
            if(stat(cfg.c_str(), &info) != 0){
                GetLogger()->Error(name_ + " Engine::LoadFromFile Stat Fail: " + cfg);
                return false;
            }
            if(S_ISDIR(info.st_mode)){
                return LoadFromDirFile(cfg);
            }else if(S_ISREG(info.st_mode)){
                return LoadFromRegFile(cfg);
            }else{
                return false;
            }
        }

        void RegisterDebugFunc(Logger::LogFunc func) { logger_.RegisterDebugFunc(func); }
        void RegisterInfoFunc (Logger::LogFunc func) { logger_.RegisterInfoFunc (func); }
        void RegisterWarnFunc (Logger::LogFunc func) { logger_.RegisterWarnFunc (func); }
        void RegisterErrorFunc(Logger::LogFunc func) { logger_.RegisterErrorFunc(func); }

};


NS_END_YME__


#endif // H_YME_ENGINE__


