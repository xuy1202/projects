
#include "hbase_handler.h"
#include <iostream>


int print_usage()
{
    std::cerr << "Usage:" << std::endl
        << "./hbase-client.exe host port get  table row"         << std::endl
        << "./hbase-client.exe host port scan table row col"     << std::endl
        << "./hbase-client.exe host port set  table row col val" << std::endl;
    return -1;
}


void result_show(const std::vector<TRowResult>& result)
{
    std::cout << ">>> total " << result.size() << " results:" << std::endl;
    std::vector<TRowResult>::const_iterator it;
    for(it=result.begin(); it!=result.end(); ++it){
        std::cout << it->row << std::endl;
        std::cout << "columns:" << std::endl;
        std::map<Text, TCell>::const_iterator mit;
        for(mit = it->columns.begin(); mit!=it->columns.end(); ++mit){
            std::cout << "    " << mit->first << ": " << mit->second.value << std::endl;
        }
    }
}


int do_get(const std::string& host, unsigned int port, const std::string& table, const std::string& row)
{
    HbaseHandler h;
    h.connect(host.c_str(), port);

    std::vector<TRowResult> result;
    std::map<Text, Text> attributes;

    h.get_one_row(result, table, row, attributes);

    result_show(result);

    h.disconnect();
    return 0;
}

int do_scan(const std::string& host, unsigned int port, const std::string& table, const std::string& row, const std::string& col)
{
    HbaseHandler h;
    h.connect(host.c_str(), port);

    std::vector<TRowResult> result;
    std::map<Text, Text> attributes;
    std::vector<Text> colvec;
    colvec.push_back(col);

    h.scan_from_row(result, table, row, colvec, attributes);

    result_show(result);

    h.disconnect();
    return 0;
}

int do_set(const std::string& host, unsigned int port, const std::string& table, const std::string& row, const std::string& col, const std::string& val)
{
    HbaseHandler h;
    h.connect(host.c_str(), port);

    std::map<Text, Text> attributes;

    h.set_one_col(table, row, col, val, attributes);

    h.disconnect();
    return 0;
}

int main(int argc, char** argv) {
    std::string host  = "";
    unsigned int port = 0;
    std::string act   = "";
    std::string table = "";
    std::string row   = "";
    std::string col   = "";
    std::string val   = "";

    if(argc >= 6){
        host = argv[1];
        port = atoi(argv[2]);
        act  = argv[3];
        table= argv[4];
        row  = argv[5];
    }
    if(argc >= 7){
        col = argv[6];
    }
    if(argc == 8){
        val = argv[7];
    }

    if(act=="get"){
        return do_get(host, port, table, row);
    }
    if(act=="scan" && !col.empty()){
        return do_scan(host, port, table, row, col);
    }
    if(act=="set" && !col.empty() && !val.empty()){
        return do_set(host, port, table, row, col, val);
    }
    return print_usage();
}

