#ifndef H_HBASE_CLIENT__
#define H_HBASE_CLIENT__

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include <iostream>

#include <boost/lexical_cast.hpp>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "Hbase.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::hadoop::hbase::thrift;


class HbaseHandler {
    public:
        HbaseHandler()
            : connected_(false)
        {
        }

        ~HbaseHandler()
        {
        }

    public:
        bool connect(const char* host, unsigned int port);
        bool disconnect();
        bool reconnect();
        inline bool isconnect();

    public:
        bool set_one_col(const std::string& table
                    , const std::string& row
                    , const std::string& col
                    , const std::string& val
                    , const std::map<Text, Text>& attributes
                    , std::string* errmsg=NULL);

        bool set_one_row(const std::string& table
                    , const std::string& row
                    , const std::vector<Mutation>& mutations
                    , const std::map<Text, Text>& attributes
                    , std::string* errmsg=NULL);

        bool set_any_row(const std::string& table
                    , const std::vector<BatchMutation>& batches
                    , const std::map<Text, Text>& attributes
                    , std::string* errmsg=NULL);

        bool get_one_row(std::vector<TRowResult>& result 
                    , const std::string& table
                    , const std::string& row
                    , const std::map<Text, Text>& attributes
                    , std::string* errmsg=NULL);

        bool get_one_row_with_col(std::vector<TRowResult>& result
                    , const std::string& table
                    , const std::string& row
                    , const std::vector<Text>& colvec
                    , const std::map<Text, Text>& attributes
                    , std::string* errmsg=NULL);

        bool get_any_row(std::vector<TRowResult>& result
                    , const std::string& table
                    , const std::vector<Text>& rowvec
                    , const std::map<Text, Text>& attributes
                    , std::string* errmsg=NULL);

        bool get_any_row_with_col(std::vector<TRowResult>& result
                    , const std::string& table
                    , const std::vector<Text>& rowvec
                    , const std::vector<Text>& colvec
                    , const std::map<Text, Text>& attributes
                    , std::string* errmsg=NULL);

        bool del_one_row(const std::string& table
                    , const std::string& row
                    , const std::map<Text, Text>& attributes
                    , std::string* errmsg=NULL);

        bool del_one_row_with_col(const std::string& table
                    , const std::string& row
                    , const std::vector<Text>& colvec
                    , const std::map<Text, Text>& attributes
                    , std::string* errmsg=NULL);

        bool scan_from_row(std::vector<TRowResult>& result
                    , const std::string& table
                    , const std::string startRow
                    , const std::vector<Text>& colvec
                    , const std::map<Text, Text>& attributes
                    , std::string* errmsg=NULL);

    private:
        bool                           connected_;
        boost::shared_ptr<TTransport>  socket_;
        boost::shared_ptr<TTransport>  transport_;
        boost::shared_ptr<TProtocol>   protocol_;
        HbaseClient*                   client_;

        std::string                    host_;
        unsigned int                   port_;
};

#endif //H_HBASE_CLIENT__



