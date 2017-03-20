#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#include <typeinfo>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "yc_repr.h"
#include "yc_str.h"
#include "yc_object.h"
#include "yc_func.h"


#define NS_BEG_YC__ namespace yc {
#define NS_END_YC__ }


NS_BEG_YC__

struct Slice: public yc::Object {
    int32_t beg;
    int32_t end;
    int32_t stp;

    Slice(int32_t b, int32_t e, int32_t s=1)
        : beg(b), end(e), stp(s)
    {
    }
};

template<typename T>
class Vector: public std::vector<T>, public yc::Object {
    public:
        Vector(): std::vector<T>() {}
        Vector(std::initializer_list<T> v): std::vector<T>(v) {}
        Vector(const std::vector<T>& v): std::vector<T>(v) {}
        ~Vector(){
            //std::cout << "del" << std::endl; 
        }

    public:
        inline void append(const T& v) { this->push_back(v); }

    public:
        inline std::string __str__() const
        {
            std::string s = "[";
            for(auto i=this->begin(); i!=this->end(); ++i){
                if(i!=this->begin()){
                    s.append(", ");
                }
                s.append(yc::str(*i));
            }
            s.append("]");
            return s;
        }

        T operator [] (int32_t idx)
        {
            if(idx >= this->size()){
                return T();
            }
            return std::vector<T>::operator [](idx);
        }

        Vector<T> operator [] (const Slice& sls)
        {
            Vector<T> v = {};
            return v;
        }

        inline void slice(int beg, int end, int step);
        inline void append(const T& v);
        inline void extend(const Vector<T>& vl);
        inline void extend(const std::vector<T>& vl);
        inline uint32_t count(const T& v);
        inline uint32_t index(const T& v);
        inline T* pop();
        inline void sort();
        inline void reverse();
};


NS_END_YC__


