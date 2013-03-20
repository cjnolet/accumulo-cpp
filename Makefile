all: build batchScannerExample scannerExample createTableExample batchWriterExample

clean:

	rm -rf target/*.o

build:
	mkdir -p target/

batchScannerExample: build
	
	g++ -DHAVE_CONFIG_H -DHAVE_NETINIET_IN_H client/*.cpp client/proxy/*.cpp examples/BatchScanner.cpp -I /usr/local/include/thrift/ -L /usr/local/lib/ -lthrift -o target/BatchScannerExample.o
	
scannerExample: build
	
	g++ -DHAVE_CONFIG_H -DHAVE_NETINIET_IN_H client/*.cpp client/proxy/*.cpp examples/Scanner.cpp -I /usr/local/include/thrift/ -L /usr/local/lib/ -lthrift -o target/ScannerExample.o
	
batchWriterExample: build
	
	g++ -DHAVE_CONFIG_H -DHAVE_NETINIET_IN_H client/*.cpp client/proxy/*.cpp examples/BatchWriter.cpp -I /usr/local/include/thrift/ -L /usr/local/lib/ -lthrift -o target/BatchWriterExample.o
	
createTableExample: build
	
	g++ -DHAVE_CONFIG_H -DHAVE_NETINIET_IN_H client/*.cpp client/proxy/*.cpp examples/CreateTable.cpp -I /usr/local/include/thrift/ -L /usr/local/lib/ -lthrift -o target/CreateTableExample.o
	