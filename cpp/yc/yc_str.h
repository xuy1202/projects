
#define NS_BEG_YC__ namespace yc {
#define NS_END_YC__ }


NS_BEG_YC__


template<typename T>
inline std::string str(const T& v)
{
    return v.__str__();
}


#ifdef _DEFINE_STR_WITH_TYPE
#define DEFINE_STR(type, format) template<>                \
    inline std::string str<type>(const type& v)            \
{                                                          \
    char buf[16] = {0};                                    \
    snprintf(buf, 16, "%s:"#format, typeid(v).name(), v);  \
    return std::string(buf);                               \
}                                               
#else
#define DEFINE_STR(type, format) template<>     \
    inline std::string str<type>(const type& v) \
{                                               \
    char buf[16] = {0};                         \
    snprintf(buf, 16, #format, v);              \
    return std::string(buf);                    \
}                                               
#endif


DEFINE_STR(char, %c);
DEFINE_STR(int, %d);
DEFINE_STR(unsigned, %u);
DEFINE_STR(long, %ld);
DEFINE_STR(unsigned long, %lu);
DEFINE_STR(long long, %lld);
DEFINE_STR(unsigned long long, %llu);
DEFINE_STR(float, %f);
DEFINE_STR(double, %lf);

#undef DEFINE_STR


template<>
inline std::string str<bool>(const bool& v)
{
    return (v ? "true": "false");
}


NS_END_YC__


