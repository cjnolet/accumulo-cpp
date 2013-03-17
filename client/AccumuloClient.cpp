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

class Mutation {
	
	string rowId;
	shared_ptr<vector<ColumnUpdate> > updates;

	public:
		Mutation(const string& rowId);
		void put(const string& colFam, const string& colQual, const string& colVis, const int64_t timestamp, const string& value);
		string& getRowId();
		shared_ptr<vector<ColumnUpdate> > getUpdates();
	
};

Mutation::Mutation(const string& rowId) {
	this->rowId = rowId;
	updates.reset(new vector<ColumnUpdate>());
}

void Mutation::put(const string& colFam, const string& colQual, const string& colVis, const int64_t timestamp, const string& value) {

	ColumnUpdate cUpdate;
	cUpdate.__set_colFamily(colFam);
	cUpdate.__set_colQualifier(colQual);
	cUpdate.__set_colVisibility(colVis);
	cUpdate.__set_timestamp(timestamp);
	cUpdate.__set_value(value);
	
	vector<ColumnUpdate> *vecUpdates = updates.get();
	
	vecUpdates->push_back(cUpdate);
}

string& Mutation::getRowId() {
	return rowId;
}

shared_ptr<vector<ColumnUpdate> > Mutation::getUpdates() {
	return updates;
}


class BatchWriter {


	shared_ptr<AccumuloProxyClient> client;
	string login;
	string writerToken;
	string tableName;
		
	public:
		
        BatchWriter(shared_ptr<AccumuloProxyClient> proxyClient, const string &login, const string &tableName,
                const int64_t maxMemory, const int64_t latencyMs, const int64_t timeoutMs, const int32_t numThreads);
        void addMutation(Mutation &mutation);
        void flush();
        void close(); 
};

BatchWriter::BatchWriter(shared_ptr<AccumuloProxyClient> proxyClient, const string &login, const string &tableName,
		const int64_t maxMemory, const int64_t latencyMs, const int64_t timeoutMs, const int32_t numThreads) {
	
	WriterOptions writerOptions;
	writerOptions.__set_maxMemory(maxMemory);
	writerOptions.__set_latencyMs(latencyMs);
	writerOptions.__set_timeoutMs(timeoutMs);
	writerOptions.__set_threads(numThreads);

	this->client = proxyClient;
	
	this->tableName = tableName;
	this->login = login;
	
	void createWriter(std::string& _return, const std::string& login, const std::string& tableName, const WriterOptions& opts);
  
	client.get()->createWriter(writerToken, login, tableName, writerOptions);
}

void BatchWriter::addMutation(Mutation &mutation) {
	
	map<string, vector<ColumnUpdate> > cells;

	vector<ColumnUpdate> *updates = mutation.getUpdates().get();
    
	string& rowId = mutation.getRowId();
	cout << writerToken;

	cells.insert(make_pair(rowId, *updates));

	client.get()->update(writerToken, cells);

	updates.clear();
}

void BatchWriter::flush() {
	
	client->flush(writerToken);
}

void BatchWriter::close() {
	client->closeWriter(writerToken);
}

class Connector {
	
	string login;
	
	shared_ptr<TSocket> socket;
	shared_ptr<TTransport> transport;
	shared_ptr<TProtocol> protocol;

	shared_ptr<AccumuloProxyClient> client;
	
	public:

		Connector(const string& host, int port, const string& username, const string& password);
		void createTable(string& tableName);
		BatchWriter createBatchWriter(const string& tableName, int64_t maxMemory, int64_t latencyMs, int64_t timeoutMs, int32_t numThreads);
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
	
	client.reset(new AccumuloProxyClient(protocol));
	
  	client.get()->login(login, username, m);

	string tableName("tableName4");
}

void Connector::createTable(string& tableName) {
	client->createTable(login, tableName, true, TimeType::MILLIS);
}

void Connector::close() {
	transport->close();
}

BatchWriter Connector::createBatchWriter(const string& tableName, const int64_t maxMemory, 
	const int64_t latencyMs, const int64_t timeoutMs, const int32_t numThreads) {
	
	BatchWriter writer(client, login, tableName, maxMemory, latencyMs, timeoutMs, numThreads);
	
	return writer;
}


int main(int argc, char* argv[]) {

	if(argc != 3) {
		cout << "Enter a hostname and port\n";	
		return 1;
	}

	
	Connector connector(argv[1], atoi(argv[2]), string("root"), string("secret"));
	BatchWriter writer = connector.createBatchWriter(string("testTable"), 500, 100, 100, 1);
	Mutation mutation(string("rowid1"));
	
	string colFam("colFam");
	string colQual("colQual");
	string colVis("");
	string val("");
	
	for(int i = 0; i < 50000; i++) {
		mutation.put( colFam, colQual, colVis, int64_t(500000), val);
	}
	
	writer.addMutation(mutation);
	
	connector.close();
	return 0;
}
