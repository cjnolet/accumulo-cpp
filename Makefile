all:
	mkdir target/
	g++ -DHAVE_CONFIG_H -DHAVE_NETINIET_IN_H client/*.cpp -I /usr/local/include/thrift/ -L /usr/local/lib/ -lthrift -o target/accumulo-cpp.out

clean:

	rm -rf target/
