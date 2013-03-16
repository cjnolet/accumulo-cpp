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

  transport->open();
  client.login(login, username, m);
  cout << "RETURNED: " + login;
  transport->close();

	
}
