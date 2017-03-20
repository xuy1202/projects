
#define NS_BEG_YC__ namespace yc {
#define NS_END_YC__ }


NS_BEG_YC__


template<typename T>
inline std::string repr(const T& v)
{
    return v.__repr__();
}


#ifdef _DEFINE_REPR_WITH_TYPE
#define DEFINE_REPR(type, format) template<>               \
    inline std::string repr<type>(const type& v)           \
{                                                          \
    char buf[16] = {0};                                    \
    snprintf(buf, 16, "%s:"#format, typeid(v).name(), v);  \
    return std::string(buf);                               \
}                                               
#else
#define DEFINE_REPR(type, format) template<>      \
    inline std::string repr<type>(const type& v) \
{                                                \
    char buf[16] = {0};                          \
    snprintf(buf, 16, #format, v);               \
    return std::string(buf);                     \
}                                               
#endif


DEFINE_REPR(char, %c);
DEFINE_REPR(int, %d);
DEFINE_REPR(unsigned, %u);
DEFINE_REPR(long, %ld);
DEFINE_REPR(unsigned long, %lu);
DEFINE_REPR(long long, %lld);
DEFINE_REPR(unsigned long long, %llu);
DEFINE_REPR(float, %f);
DEFINE_REPR(double, %lf);

#undef DEFINE_REPR


template<>
inline std::string repr<bool>(const bool& v)
{
    return (v ? "true": "false");
}


NS_END_YC__


