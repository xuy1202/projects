#ifndef H_FILE__
#define H_FILE__

#include <fstream>
#include <iostream>

#define NS_BEG_FILE__ namespace file {
#define NS_END_FILE__ }

#define NS_BEG_FILE_PHP__ namespace php {
#define NS_END_FILE_PHP__ }

#define NS_BEG_FILE_PY__ namespace py {
#define NS_END_FILE_PY__ }

#define NS_BEG_OS_PATH__ namespace os { namespace path {
#define NS_END_OS_PATH__ } }


NS_BEG_FILE__

NS_BEG_FILE_PHP__

bool file_get_contents(const std::string& path, std::string& out, std::ios_base::openmode flags=std::ios::binary);
bool file_put_contents(const std::string& path, const std::string& out, std::ios_base::openmode flags=std::ios::binary|std::ios::trunc);

NS_END_FILE_PHP__


NS_BEG_FILE_PY__
NS_BEG_OS_PATH__

bool exists(const std::string& path);
bool dirname(const std::string& path);
bool basename(const std::string& path);
bool isabs(const std::string& path);
bool isdir(const std::string& path);
bool isfile(const std::string& path);
bool islink(const std::string& path);
bool ismount(const std::string& path);
bool realpath(const std::string& path);

NS_END_OS_PATH__
NS_END_FILE_PY__

NS_END_FILE__



#endif // H_FILE__
