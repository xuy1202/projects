

INCLUDE_DIR= -I/home/netlab/dev/include/ -I/home/netlab/dev/include/thrift/

LIB_DIR=/home/netlab/dev/lib/

GEN_SRC = ./gen-cpp/Hbase.cpp \
		  ./gen-cpp/Hbase_types.cpp \
		  ./gen-cpp/Hbase_constants.cpp


test: hbase-client.cc hbase_handler.cc
	g++ -o hbase-client.exe -I./ -I./gen-cpp $(INCLUDE_DIR) -L${LIB_DIR} -Wl,-rpath=$(LIB_DIR) -lthrift hbase-client.cc hbase_handler.cc ${GEN_SRC} -DHAVE_CONFIG_H

clean:
	rm -rf demo
	rm -rf testput
	rm -rf testget
	rm -rf *.exe

