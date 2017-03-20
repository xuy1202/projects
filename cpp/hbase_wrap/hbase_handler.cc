
#include "hbase_handler.h"


bool HbaseHandler::connect(const char* host, unsigned int port)
{
    host_ = host;
    port_ = port;

    socket_.reset(new (std::nothrow) TSocket(host, port));
    if(socket_.get() == NULL){
        return false;
    }
    transport_.reset(new (std::nothrow) TBufferedTransport(socket_));
    if(transport_.get() == NULL){
        return false;
    }
    protocol_.reset(new (std::nothrow) TBinaryProtocol(transport_));
    if(protocol_.get() == NULL){
        return false;
    }
    client_ = new (std::nothrow) HbaseClient(protocol_);
    if(client_ == NULL){
        return false;
    }
    connected_ = false;
    try {
        transport_->open();
        connected_ = true;
    } catch (const TException &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
    return connected_;
}


bool HbaseHandler::disconnect()
{
    try {
        transport_->close();
        connected_ = false;
        return true;
    } catch (const TException &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return false;
    }
}


bool HbaseHandler::reconnect()
{
    return (disconnect() && connect(host_.c_str(), port_));
}


inline bool HbaseHandler::isconnect()
{
    return connected_;
} 


#define HH_SAFE_CALL(f, i ...)              \
    bool __call_ret = false;                \
do{                                         \
    if(! isconnect()){                      \
        break;                              \
    }                                       \
    try{                                    \
        client_->f(i);                      \
        __call_ret = true;                  \
    } catch (const TTransportException& e){ \
        if(errmsg){                         \
            errmsg->assign(e.what());       \
        }                                   \
        __call_ret = false;                 \
    }                                       \
}while(0);                                  \


bool HbaseHandler::set_one_col(const std::string& table
            , const std::string& row
            , const std::string& col
            , const std::string& val
            , const std::map<Text, Text>& attributes
            , std::string* errmsg)
{
    std::vector<Mutation> mutations;
    mutations.push_back(Mutation());
    mutations.back().column = col;
    mutations.back().value  = val;
    return set_one_row(table, row, mutations, attributes, errmsg);
}

bool HbaseHandler::set_one_row(const std::string& table
            , const std::string& row
            , const std::vector<Mutation>& mutations
            , const std::map<Text, Text>& attributes
            , std::string* errmsg)
{
    HH_SAFE_CALL(mutateRow, table, row, mutations, attributes);
    return __call_ret;
}

bool HbaseHandler::set_any_row(const std::string& table
            , const std::vector<BatchMutation>& batches
            , const std::map<Text, Text>& attributes
            , std::string* errmsg)
{
    HH_SAFE_CALL(mutateRows, table, batches, attributes);
    return __call_ret;
}

bool HbaseHandler::get_one_row(std::vector<TRowResult>& result 
            , const std::string& table
            , const std::string& row
            , const std::map<Text, Text>& attributes
            , std::string* errmsg)
{
    HH_SAFE_CALL(getRow, result, table, row, attributes);
    return __call_ret;
}

bool HbaseHandler::get_one_row_with_col(std::vector<TRowResult>& result
            , const std::string& table
            , const std::string& row
            , const std::vector<Text>& colvec
            , const std::map<Text, Text>& attributes
            , std::string* errmsg)
{
    HH_SAFE_CALL(getRowWithColumns, result, table, row, colvec, attributes); 
    return __call_ret;
}

bool HbaseHandler::get_any_row(std::vector<TRowResult>& result
            , const std::string& table
            , const std::vector<Text>& rowvec
            , const std::map<Text, Text>& attributes
            , std::string* errmsg)
{
    HH_SAFE_CALL(getRows, result, table, rowvec, attributes);
    return __call_ret;
}

bool HbaseHandler::get_any_row_with_col(std::vector<TRowResult>& result
            , const std::string& table
            , const std::vector<Text>& rowvec
            , const std::vector<Text>& colvec
            , const std::map<Text, Text>& attributes
            , std::string* errmsg)
{
    HH_SAFE_CALL(getRowsWithColumns, result, table, rowvec, colvec, attributes);
    return __call_ret;
}

bool HbaseHandler::del_one_row(const std::string& table
            , const std::string& row
            , const std::map<Text, Text>& attributes
            , std::string* errmsg)
{
    HH_SAFE_CALL(deleteAllRow, table, row, attributes);
    return __call_ret;
}

bool HbaseHandler::del_one_row_with_col(const std::string& table
            , const std::string& row
            , const std::vector<Text>& colvec
            , const std::map<Text, Text>& attributes
            , std::string* errmsg)
{
    std::vector<Mutation> mutations;
    std::vector<Text>::const_iterator it;
    for(it=colvec.begin(); it!=colvec.end(); ++it){
        mutations.push_back(Mutation());
        mutations.back().column = *it;
        mutations.back().isDelete = true;
    }
    HH_SAFE_CALL(mutateRow, table, row, mutations, attributes);
    return __call_ret;
}

#undef HH_SAFE_CALL


bool HbaseHandler::scan_from_row(std::vector<TRowResult>& result
            , const std::string& table
            , const std::string startRow
            , const std::vector<Text>& colvec
            , const std::map<Text, Text>& attributes
            , std::string* errmsg)
{
    if (! isconnect()) {
        return false;
    }
    int scanner = client_->scannerOpenWithPrefix(table, startRow, colvec, attributes);
    try {
        client_->scannerGetList(result, scanner, 500); // set max 500 rows
        client_->scannerClose(scanner);
        return true;
    } catch (const IOError &e) {
        if(errmsg){
            errmsg->assign(e.what());
        }
        client_->scannerClose(scanner);
        return false;
    } catch (const TTransportException &e) {
        if(errmsg){
            errmsg->assign(e.what());
        }
        client_->scannerClose(scanner);
        connected_ = false;
        return false;
    }
}


