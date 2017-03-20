

OUTPUT_DIR=$(shell pwd)

PROTOS := $(wildcard *.proto)
SRCS   := $(patsubst %.proto, $(OUTPUT_DIR)/%.pb.cc, $(PROTOS))
PYSRCS := $(patsubst %.proto, $(OUTPUT_DIR)/%_pb2.py, $(PROTOS))


all : $(SRCS) $(PYSRCS)
	
t :
	@echo $(OUTPUT_DIR)
	@echo $(PROTOS)
	@echo $(SRCS)

clean :
	@rm -rf *.pb.cc *.pb.h
	@rm -rf *.d 
	@rm -rf *.o 
	@rm -rf *.py
	@rm -rf *.pyc
	@rm -rf *.pyo

$(OUTPUT_DIR)/%.pb.cc : %.proto
	protoc --cpp_out=$(OUTPUT_DIR) $<

$(OUTPUT_DIR)/%_pb2.py : %.proto
	protoc --python_out=$(OUTPUT_DIR) $<
