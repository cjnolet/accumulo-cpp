#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TCompactProtocol.h>
#include "proxy/AccumuloProxy.h"

using namespace std;
using namespace accumulo;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace boost;

int main(int argc, char* argv[]) {

  if(argc != 3) {
	cout << "Enter a hostname and port\n";	
	return 1;
  }

  shared_ptr<TSocket> socket(new TSocket(argv[1], atoi(argv[2])));
  shared_ptr<TTransport> transport(new TFramedTransport(socket));
  shared_ptr<TProtocol> protocol(new TCompactProtocol(transport));

  string passKey("password");
  string passWord("secret");
  string username("root");

  string login;

  map<string, string> m;
  m.insert(make_pair(passKey, passWord));

  AccumuloProxyClient client(protocol);

  string tableName("testTable");

  transport->open();
  client.login(login, username, m);
  // client.createTable(login, tableName, true, TimeType::MILLIS);

  WriterOptions writerOptions;
  writerOptions.__set_maxMemory(5000);
  writerOptions.__set_latencyMs(5000);
  writerOptions.__set_timeoutMs(200000);
  writerOptions.__set_threads(5);

  string writer;
  client.createWriter(writer, login, tableName, writerOptions);

	ColumnUpdate cUpdate;
	cUpdate.__set_colFamily(string("colFam"));
	cUpdate.__set_colQualifier(string("colQual"));
	cUpdate.__set_colVisibility(string(""));
	cUpdate.__set_timestamp(5000);
	cUpdate.__set_value(string(""));

   map<string, vector<ColumnUpdate> > cells;

  string rowId("rowId");

  ColumnUpdate upArr[]  = { cUpdate };

  vector<ColumnUpdate> ud(upArr, upArr + sizeof(upArr) / sizeof(ColumnUpdate));
  
  cells.insert(make_pair(rowId, ud));
  
  client.update(writer, cells);

  transport->close();
}
