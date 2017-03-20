/*
 *                              ret: long   double   string
 *                                   equal  equal    equal
 *                                   inspan inspan
 *
 * string.equal(str1,str2)      0,1 
 * string.startswith(str,str2)  0,1
 * string.endswith(str)         0,1
 * string.find(str)             0,1
 * string.regex                 0,1
 * string.index                 string 
 *
 * number.fitfunc(1,2)          double
 *
 * number.equal(1,2)            false
 * number.inenum(1,2,3)         true
 * number.inspan(1-3,5-6)       false
 *
 * ffunc.fit11(daynum)          a+bx=(0.5y,3y)
 *
 * example: 
 *     1: dip       equal        1.1.1.1
 *     1: dport     equal        80 
 *     1: pkgs      in_span      30-100
 *     1: flow      fitfunc_12   0.5*x*x + 0.1*x + 138
 *
 */

#ifndef H_YME__
#define H_YME__

#include <yme/engine.hpp>


NS_BEG_YME__


template<class RawType, class DatType, class RuleType=Rule, class RetType=MatchedResult<DatType> >
class EngineChain{
    private:
        typedef yme::Engine<RawType, DatType, RuleType, RetType> EngineType;

        std::vector<EngineType*>   chain_;
        Logger                    logger_; 
        bool                  full_debug_;

    public:
#ifdef _YME_DEBUG
        EngineChain(): full_debug_(true){}
#else
        EngineChain(): full_debug_(false){}
#endif

        bool AppendEngine(EngineType* eng)
        {
            chain_.push_back(eng);
            return true;
        }

        inline const Logger* GetLogger() const { return &logger_;    }
        inline void SetFullDebug(bool b)       { full_debug_ = b;    }
        inline bool GetFullDebug() const       { return full_debug_; }

    public:
        bool MatchChain(const RawType& rdata)
        {
            RetType result;
            if(! result.Data.Feed(rdata)){
#ifdef _YME_DEBUG
                if(GetFullDebug()){
                    GetLogger()->Debug("Match::ParseFail");
                }
#endif
                result.MatchedCode = RetType::kParseFail;
                return false;
            }
            if(! MatchChain(result)){
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

        bool MatchChain(RetType& result)
        {
            assert(chain_.size() > 0);

            for(auto it=chain_.begin(); it!=chain_.end(); ++it){
                bool ret = (*it)->Match(result);
                if(! ret){
                    return false;
                }
                if(result.MatchedCode == RetType::kMatchNone){
                    continue;
                }else{
                    // match one, then return
                    return true;
                }
            }
            // looping all the engine while match none
            return true;
        }
};


NS_END_YME__


#endif // H_YME__



