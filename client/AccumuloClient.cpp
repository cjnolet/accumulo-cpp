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

class Connector {
	
	string login;
	
	shared_ptr<TSocket> socket;
	shared_ptr<TTransport> transport;
	shared_ptr<TProtocol> protocol;

	AccumuloProxyClient *client;
	
	public:

		Connector(const string& host, int port, const string& username, const string& password);
		void createTable(const string& tableName);
		void createBatchWriter(string &tableName, uint32_t maxMemory, uint32_t latencyMs, uint32_t timeoutMs, uint32_t numThreads);
		void close();
	
};

Connector::Connector(const string& host, int port, const string& username, const string& password) {

	shared_ptr<TSocket> newSocket(new TSocket(host, port));
	shared_ptr<TTransport> newTransport(new TFramedTransport(newSocket));
	shared_ptr<TProtocol> newProtocol(new TCompactProtocol(newTransport));

	socket = newSocket;
	transport = newTransport;
	protocol = newProtocol;

	transport->open();

	string passKey("password");

  	map<string, string> m;
  	m.insert(make_pair(passKey, password));
	
	AccumuloProxyClient newClient(protocol);
	
	client = &newClient;
  	client->login(login, username, m);
}

void Connector::createTable(const string& tableName) {
	client->createTable(login, tableName, true, TimeType::MILLIS);
}

void Connector::close() {
	transport->close();
}

class Mutation {
	
	string rowId;

	public:
		Mutation(string &rowId);
		void put(string& colFam, string& colQual, string& colVis, uint32_t timestamp, string& value);
	
};

Mutation::Mutation(string& rowId) {
	this->rowId = rowId;
}


class BatchWriter {

	AccumuloProxyClient *client;
	string login;
	string writerToken;
	string tableName;
	
	public:
		
		BatchWriter(AccumuloProxyClient *client, string &login, string &tableName,
				uint32_t maxMemory, uint32_t latencyMs, uint32_t timeoutMs, uint32_t numThreads);
		void addMutation(Mutation *mutation);
		void flush();
		void close(); 
};

BatchWriter::BatchWriter(AccumuloProxyClient *client, string &login, string &tableName,
		uint32_t maxMemory, uint32_t latencyMs, uint32_t timeoutMs, uint32_t numThreads) {
	
	WriterOptions writerOptions;
	writerOptions.__set_maxMemory(maxMemory);
	writerOptions.__set_latencyMs(latencyMs);
	writerOptions.__set_timeoutMs(timeoutMs);
	writerOptions.__set_threads(numThreads);
	
	this->client = client;
	this->tableName = tableName;
	this->login = login;

	client->createWriter(writerToken, login, tableName, writerOptions);
}

void BatchWriter::flush() {
	
	client->flush(writerToken);
}

void BatchWriter::close() {
	client->closeWriter(writerToken);
}


void Mutation::put(string& colFam, string& colQual, string& colVis, uint32_t timestamp, string& value) {

	ColumnUpdate cUpdate;
	cUpdate.__set_colFamily(string("colFam"));
	cUpdate.__set_colQualifier(string("colQual"));
	cUpdate.__set_colVisibility(string(""));
	cUpdate.__set_timestamp(5000);
	cUpdate.__set_value(string(""));
}


int main(int argc, char* argv[]) {

	if(argc != 3) {
		cout << "Enter a hostname and port\n";	
		return 1;
	}
	
	Connector connector(argv[1], atoi(argv[2]), string("root"), string("secret"));
	connector.close();



	//   string tableName("testTable");
	// 
	// 
	//   // client.createTable(login, tableName, true, TimeType::MILLIS);
	// 
	// 
	//   WriterOptions writerOptions;
	//   writerOptions.__set_maxMemory(5000);
	//   writerOptions.__set_latencyMs(5000);
	//   writerOptions.__set_timeoutMs(200000);
	//   writerOptions.__set_threads(5);
	// 
	//   string writer;
	//   client.createWriter(writer, login, tableName, writerOptions);
	// 
	// ColumnUpdate cUpdate;
	// cUpdate.__set_colFamily(string("colFam"));
	// cUpdate.__set_colQualifier(string("colQual"));
	// cUpdate.__set_colVisibility(string(""));
	// cUpdate.__set_timestamp(5000);
	// cUpdate.__set_value(string(""));
	// 
	//    map<string, vector<ColumnUpdate> > cells;
	// 
	//   string rowId("rowId");
	// 
	//   ColumnUpdate upArr[]  = { cUpdate };
	// 
	//   vector<ColumnUpdate> ud(upArr, upArr + sizeof(upArr) / sizeof(ColumnUpdate));
	//   
	//   cells.insert(make_pair(rowId, ud));
	//   
	//   for(int i = 0; i < 50000; i++) {
	//     client.update(writer, cells);
	//   }
	// 
	// 
	// 
	//   transport->close();
  return 0;
}
