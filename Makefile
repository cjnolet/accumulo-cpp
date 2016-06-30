
TARGETS=target/batchScannerExample target/scannerExample \
	target/createTableExample target/batchWriterExample

all: target gen-cpp ${TARGETS}

CXXFLAGS += -Igen-cpp

LIBS=-lthrift

clean:
	rm -rf target/*

target:
	mkdir -p target/

gen-cpp: proxy.thrift
	rm -rf $@
	mkdir $@
	thrift --allow-64bit-consts --out $@ --gen cpp proxy.thrift

CLIENT_OBJS=client/api/TableOperations.o client/AccumuloAPI.o \
	gen-cpp/AccumuloProxy.o gen-cpp/proxy_types.o \
	gen-cpp/proxy_constants.o

target/batchScannerExample: ${CLIENT_OBJS} examples/BatchScanner.o
	${CXX} ${CXXFLAGS} examples/BatchScanner.o ${CLIENT_OBJS} \
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

