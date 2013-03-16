all:
	rm -rf target/*.o
	mkdir -p target/
	g++ -DHAVE_CONFIG_H -DHAVE_NETINIET_IN_H client/*.cpp client/proxy/*.cpp -I /usr/local/include/thrift/ -L /usr/local/lib/ -lthrift -o target/accumuloclient.o

clean:

	rm -rf target/
