#include "file.h"

NS_BEG_FILE__
NS_BEG_FILE_PHP__


bool file_get_contents(const std::string& path, std::string& out, std::ios_base::openmode flags)
{
    std::ifstream in(path.c_str(), flags);
    if(! in.good()){
        return false;
    }
    in.seekg(0, std::ios::end);
    std::streampos len = in.tellg();
    in.seekg(0, std::ios::beg);

    out.reserve(len);
    out.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    in.close();
    return true;
}


bool file_put_contents(const std::string& path, const std::string& out, std::ios_base::openmode flags)
{
    std::ofstream of(path.c_str(), flags);
    if(! of.good()){
        return false;
    }
    of << out;
    of.close();
    return true;
}


NS_END_FILE__
NS_END_FILE_PHP__


