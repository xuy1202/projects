
#include "base64.h"  


NS_BEG_BASE64__


static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  

static inline bool char_is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
} 


void encode(const char* input, unsigned int in_len, std::string& output)
{
    output.clear();
    output.reserve(in_len > 3 ? in_len*2: 4);
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned char char_array_3[3];  
    unsigned char char_array_4[4];  
    // loop process
    while(in_len--){
        char_array_3[i++] = *(input++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;  
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);  
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);  
            char_array_4[3] = char_array_3[2] & 0x3f;  
            for (i = 0; (i <4) ; i++){
                output += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }
    // do the rest
    if(i){
        for(j = i; j < 3; j++){
            char_array_3[j] = 0x0;  
        }
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;  
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);  
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);  
        char_array_4[3] = char_array_3[2] & 0x3f;  
        for (j = 0; (j < i + 1); j++){
            output += base64_chars[char_array_4[j]];
        }
        while ((i++ < 3)){
            output += '=';
        }
    }
}

void encode(const std::string& input, std::string& output)
{
    encode(input.data(), input.size(), output);
}

std::string encode(const char* input, unsigned int in_len)
{
    std::string output;
    encode(input, in_len, output);
    return output;
}

std::string encode(const std::string& input)
{
    return encode(input.data(), input.size());
}


void decode(const char* input, unsigned int in_len, std::string& output) 
{
    output.reserve(in_len);
    unsigned int i = 0;
    unsigned int j = 0; 
    unsigned int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];  

    while (in_len-- && ( input[in_] != '=') && char_is_base64(input[in_])){
        char_array_4[i++] = input[in_];
        in_++;
        if (i ==4) {  
            for (i = 0; i <4; i++){
              char_array_4[i] = (unsigned char)base64_chars.find(char_array_4[i]);
            }
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);  
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);  
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            for (i = 0; (i < 3); i++){
              output += char_array_3[i];
            }
            i = 0;  
        }  
    }  
    if (i) {  
        for (j = i; j <4; j++){
          char_array_4[j] = 0;
        }
        for (j = 0; j <4; j++){
          char_array_4[j] = (unsigned char)base64_chars.find(char_array_4[j]);
        }
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);  
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);  
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];  
        for (j = 0; (j < i - 1); j++){
            output += char_array_3[j];
        }
    }
}

void decode(const std::string& input, std::string& output)
{
    decode(input.data(), input.size(), output);
}

std::string decode(const char* input, unsigned int in_len)
{
    std::string output;
    decode(input, in_len, output);
    return output;
}

std::string decode(const std::string& input)
{
    return decode(input.data(), input.size());    
}


NS_END_BASE64__


