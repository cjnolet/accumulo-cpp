
TARGETS=target/batchScannerExample target/scannerExample \
	target/createTableExample target/batchWriterExample

all: target ${TARGETS}

CXXFLAGS=-DHAVE_CONFIG_H -DHAVE_NETINIET_IN_H
CXXFLAGS += -I/usr/local/include/thrift/
CXXFLAGS += -L/usr/local/lib/
LIBS=-lthrift

clean:
	rm -rf target/*

target:
	mkdir -p target/

CLIENT_OBJS=client/api/TableOperations.o client/AccumuloAPI.o \
	client/proxy/proxy_types.o client/proxy/AccumuloProxy.o \
	client/proxy/proxy_constants.o

target/batchScannerExample: ${CLIENT_OBJS} examples/BatchScanner.o
	${CXX} ${CXXFLAGS} ${CLIENT_OBJS} examples/BatchScanner.o \
		${LIBS} -o $@

target/scannerExample: ${CLIENT_OBJS} examples/Scanner.o
	${CXX} ${CXXFLAGS} ${CLIENT_OBJS} examples/Scanner.o ${LIBS} \
		-o $@

target/batchWriterExample: ${CLIENT_OBJS} examples/BatchWriter.o
	${CXX} ${CXXFLAGS} ${CLIENT_OBJS} examples/BatchWriter.o ${LIBS} \
		-o $@

target/createTableExample: ${CLIENT_OBJS} examples/CreateTable.o
	${CXX} ${CXXFLAGS} ${CLIENT_OBJS} examples/CreateTable.o \
		${LIBS} -o $@

