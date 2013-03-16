
all:
	g++ -DHAVE_CONFIG_H -DHAVE_NETINIET_IN_H *.cpp -I /usr/local/include/thrift/ -L /usr/local/lib/ -lthrift -o accumulo-cpp.out
