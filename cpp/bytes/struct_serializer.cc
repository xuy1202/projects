
#include <inttypes.h>
#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// simplest log message object with zero-copy serialize && parse function
//   serialization should be called right after attributes setting
//   and log data only can be freed after all post-parse functions
// structure:
//   alllen catlen filelen msglen line time_ms level cat \0 file \0  msg \0
//     4      4      4       4      4     8      1   
//  extra size = 4 + 4 + 4 + 4 + 4 + 8 + 1 + 3(\0) = 32
struct LogObject{
    uint32_t all_len;
    uint32_t cat_len;
    uint32_t file_len;
    uint32_t msg_len;
    uint32_t line;
    uint64_t time_ms;
    uint8_t  level;
    const char* cat;
    const char* file;
    const char* msg;

    LogObject()
        : all_len(0), cat_len(0), file_len(0)
          , msg_len(0), line(0), time_ms(0)
          , level(0), cat(NULL), file(NULL), msg(NULL)
    {
    }

    bool parse(const char* log, uint32_t len)
    {
        const char* ptr = log;

        memcpy((char*)(&all_len) , ptr, 4); ptr += 4;
        memcpy((char*)(&cat_len) , ptr, 4); ptr += 4;
        memcpy((char*)(&file_len), ptr, 4); ptr += 4;
        memcpy((char*)(&msg_len) , ptr, 4); ptr += 4;
        memcpy((char*)(&line)    , ptr, 4); ptr += 4;
        memcpy((char*)(&time_ms) , ptr, 8); ptr += 8;
        memcpy((char*)(&level)   , ptr, 1); ptr += 1;

        cat  = ptr; ptr += cat_len + 1;
        file = ptr; ptr += file_len + 1;
        msg  = ptr; ptr += msg_len + 1;

        assert(*ptr == 0x0);
        return true;
    }

    bool serialize(char* out)
    {
        char* ptr = out;

        memcpy(ptr, (char*)(&all_len) , 4); ptr += 4;
        memcpy(ptr, (char*)(&cat_len) , 4); ptr += 4;
        memcpy(ptr, (char*)(&file_len), 4); ptr += 4;
        memcpy(ptr, (char*)(&msg_len) , 4); ptr += 4;
        memcpy(ptr, (char*)(&line)    , 4); ptr += 4;
        memcpy(ptr, (char*)(&time_ms) , 8); ptr += 8;
        memcpy(ptr, (char*)(&level)   , 1); ptr += 1;

        memcpy(ptr, cat, cat_len); 
        ptr += cat_len;
        *ptr = 0x0; ptr += 1;
        memcpy(ptr, file, file_len); 
        ptr += file_len;
        *ptr = 0x0; ptr += 1;
        memcpy(ptr, msg, msg_len);
        ptr += msg_len;
        *ptr = 0x0; ptr += 1;

        return true;
    }

    inline void calc_all_len() { all_len = 32 + cat_len + file_len + msg_len; }

    inline void add_level(uint8_t l) { level = l;   calc_all_len(); }
    inline void add_line(uint32_t l) { line = l;    calc_all_len(); }
    inline void add_time(uint64_t t) { time_ms = t; calc_all_len(); }
    inline void add_cat(const char* c, uint32_t l)  { cat = c; cat_len = l;   calc_all_len(); } 
    inline void add_file(const char* f, uint32_t l) { file = f; file_len = l; calc_all_len(); }
    inline void add_msg(const char* m, uint32_t l)  { msg = m; msg_len = l;   calc_all_len(); }
};


void char_show(const char* in, uint32_t len)
{
    static const int turn_limit = 16;
    char buf[turn_limit * 4 + 4] = {0};
    char* ptr = buf;
    for(uint32_t i=0; i<len; ++i){
        if(i % turn_limit == 0 && ptr != buf){
            printf("|  %s\n", buf);
            ptr = buf;
        }
        if(*(in + i) >= ' '){
            printf("%*c", 2, *(in+i));
        }else{
            printf(" .");
        }
        uint8_t n = (uint8_t)(unsigned char)(*(in + i));
        sprintf(ptr, "%*u", 4, n); ptr += 4;
        if(i % (turn_limit >> 1) == (turn_limit >> 1) - 1){
            printf("  ");
            sprintf(ptr, "  "); ptr += 2;
        }
    }
    int i = 0;
    while(i++ < (turn_limit - (len % turn_limit))*2 + 4) printf(" ");
    printf("|  %s\n", buf);
}


int main(void)
{
    LogObject obj;
    obj.add_level(1);
    obj.add_cat("test", 4);
    obj.add_file("main.cc", 7);
    obj.add_line(102);
    obj.add_time(999999999);
    obj.add_msg("hello world", 11);

    std::cout << obj.all_len  << std::endl;
    std::cout << obj.cat_len  << std::endl;
    std::cout << obj.file_len << std::endl;
    std::cout << obj.msg_len  << std::endl;
    std::cout << obj.time_ms  << std::endl;
    std::cout << (int)obj.level    << std::endl;
    std::cout << obj.cat      << std::endl;
    std::cout << obj.file     << std::endl;
    std::cout << obj.msg      << std::endl;

    char* out = (char*)malloc(obj.all_len);
    obj.serialize(out);

    char_show(out, obj.all_len);

    LogObject new_obj;
    new_obj.parse(out, obj.all_len);

    std::cout << new_obj.all_len  << std::endl;
    std::cout << new_obj.cat_len  << std::endl;
    std::cout << new_obj.file_len << std::endl;
    std::cout << new_obj.msg_len  << std::endl;
    std::cout << new_obj.time_ms  << std::endl;
    std::cout << (int)new_obj.level    << std::endl;
    std::cout << new_obj.cat      << std::endl;
    std::cout << new_obj.file     << std::endl;
    std::cout << new_obj.msg      << std::endl;

    return 0;
}



