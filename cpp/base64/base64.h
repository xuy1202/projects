

#include <iostream>  


#define NS_BEG_BASE64__ namespace base64 {
#define NS_END_BASE64__ }


NS_BEG_BASE64__


void encode(const std::string& input, std::string& output);

std::string encode(const std::string& input);

void encode(const char* input, unsigned int len, std::string& output);

std::string encode(const char* input, unsigned int len);


void decode(const std::string& input, std::string& output);

std::string decode(const std::string& input);

void decode(const char* input, unsigned int len, std::string& output);

std::string decode(const char* input, unsigned int len);


//#ifdef __cplusplus
//extern "C" {
//    void base64_encode(const char* input, unsigned int in_len, char* output, unsigned int out_len);
//    char* base64_encode(const char* input, unsigned int in_len);
//
//    void base64_decode(const char* input, unsigned int in_len, char* output, unsigned int out_len);
//    char* base64_decode(const char* input, unsigned int in_len);
//}
//#endif


NS_END_BASE64__
