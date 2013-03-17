#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TCompactProtocol.h>
#include "proxy/AccumuloProxy.h"
#include "AccumuloAPI.h"


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

void Mutation::clear() {
	
	updates.reset(new vector<ColumnUpdate>());
}


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

	cells.insert(make_pair(rowId, *updates));

	client.get()->update(writerToken, cells);
	
	mutation.clear();
}

void BatchWriter::flush() {
	
	client->flush(writerToken);
}

void BatchWriter::close() {
	client->closeWriter(writerToken);
}

ScannerIterator::ScannerIterator(shared_ptr<AccumuloProxyClient> proxyClient, const string& login, const string& tableName, 
				ScanOptions options) {
					
	this->client = proxyClient;
	this->options = options;
	this->tableName = tableName;
	this->login = login;
	
	client.get()->createScanner(scannerToken, login, tableName, options);
}

Scanner::Scanner(shared_ptr<AccumuloProxyClient> proxyClient, const string& login, const string& tableName, 
		const set<string> authorizations) {

		options.__set_authorizations(authorizations);

		this->client = proxyClient;
		this->login = login;
		this->tableName;
}

Connector::Connector(const string& host, int port, const string& username, const string& password) {

	shared_ptr<TSocket> newSocket(new TSocket(host, port));
	shared_ptr<TTransport> newTransport(new TFramedTransport(newSocket));
	shared_ptr<TProtocol> newProtocol(new TCompactProtocol(newTransport));

	socket = newSocket;
	transport = newTransport;
	protocol = newProtocol;

	transport->open();

	string passkey("password");

  	map<string, string> m;
  	m.insert(make_pair(passkey, password));
	
	client.reset(new AccumuloProxyClient(protocol));
	
  	client.get()->login(login, username, m);
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
