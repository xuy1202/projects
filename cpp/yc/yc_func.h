
#define NS_BEG_YC__ namespace yc {
#define NS_END_YC__ }


NS_BEG_YC__


template<typename T, typename DST=FILE*>
inline void print(const T& v, DST fd=stdout)
{
    fprintf(fd, "%s\n", yc::str(v).c_str());
}


inline void print(const std::string& s, FILE* fd=stdout)
{
    fprintf(fd, "%s\n", s.c_str());
}


//inline map(yc::Object* obj, yc::Func func)
//{
//
//}
//inline filter();
//inline reduct();
//
//bool all();
//bool any(); 
//std::string bin();
//bool callable
//char chr
//hasattr
//getattr
//delattr
//setattr
//dir
//enumerate
//hash
//hex
//id
//isinstance
//issubclass
//iter
//len
//max
//min
//sum
//next
//oct
//ord
//property
//slice
//reversed
//sorted
//type
//vars


NS_END_YC__


