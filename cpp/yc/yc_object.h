
#define NS_BEG_YC__ namespace yc {
#define NS_END_YC__ }


NS_BEG_YC__


class Object{
    public:
        virtual inline std::string __str__() const
        {
            char buf[64] = {0};
            snprintf(buf, 64, "<str:%p %s>", this, typeid(*this).name());
            return std::string(buf);
        }

        virtual inline std::string __repr__() const
        {
            char buf[64] = {0};
            snprintf(buf, 64, "<repr:%p %s>", this, typeid(*this).name());
            return std::string(buf);
        }
};


//template<typename T>
//class DynamicObject: public Object {
//    public:
//        friend class Property;
//
//    public:
//        std::map<std::string, ANY> __dict__;
//
//    public:
//        template<typename T>
//            inline T& operator -> (const std::string& key)
//            {
//            }
//
//        template<typename T>
//            inline T getattr(const std::string& key)
//            {
//            }
//
//        template<typename T>
//            inline T setattr(const std::string& key, const T& val)
//            {
//
//            }
//
//        template<typename T=void>
//        bool hasattr(const std::string& key)
//        {
//            static typeid(void) void_type;
//            if(typeid(T) == void_type){
//                return (__dict__.find(key) != __dict__.end());
//            }
//            auto it = __dict__.find(key);
//            bool ret = true;
//            ret = ret && it != __dict__.end();
//            ret = ret && typeid<T> = it->second.type(); 
//            return ret;
//        }
//
//        void delattr(const std::string& key)
//        {
//        }
//};


NS_END_YC__


