#ifndef H_HASHLIB__
#define H_HASHLIB__

#include "md5.h"

#include <iostream>

#define NS_BEG_HASHLIB__ namespace hashlib {
#define NS_END_HASHLIB__ }

NS_BEG_HASHLIB__

class Md5 {
    public:
        Md5()
        {
            md5_init(&md5_ctx_);
        }

    public:
        inline void update(const unsigned char* data, unsigned int len)
        {
            md5_append(&md5_ctx_, data, len);
        }

        inline void update(const std::string& data)
        {
            return update((const unsigned char*)data.data(), (unsigned int)data.size());
        }

        inline void digest(std::string& output)
        {
            unsigned char buffer[17] = {0};
            md5_finish(&md5_ctx_, buffer);
            output.assign((const char*)buffer);
        }

        inline std::string digest()
        {
            std::string output;
            digest(output);
            return output;
        }

        inline void hexdigest(std::string& output) 
        {
           unsigned char buffer[17] = {0};
           unsigned char hexbuffer[33] = {0};
           md5_finish(&md5_ctx_, buffer);
           int i = 0;
           int j = 0;
           for(i=j=0; i<16; i++) {
               char c;
               c = (buffer[i] >> 4) & 0xf;
               c = (c>9) ? c+'a'-10 : c + '0';
               hexbuffer[j++] = c;
               c = (buffer[i] & 0xf);
               c = (c>9) ? c+'a'-10 : c + '0';
               hexbuffer[j++] = c;
           }
           output.assign((const char*)hexbuffer);
        }

        inline std::string hexdigest()
        {
            std::string output;
            hexdigest(output);
            return output;
        }

    private:
        md5_state_t md5_ctx_;
};


inline std::string md5sum(const unsigned char* data, unsigned int len)
{
    std::string ret;
    Md5 md5;
    md5.update(data, len);
    md5.hexdigest(ret);
    return ret;
}


inline std::string md5sum(const std::string& data)
{
    return md5sum((const unsigned char*)data.data(), (unsigned int)data.size());
}


NS_END_HASHLIB__


#endif // H_HASHLIB__


