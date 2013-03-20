#include <stdlib.h>
#include <iostream>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TCompactProtocol.h>
#include "proxy/AccumuloProxy.h"

#ifndef ACCUMULO_API_H
#define ACCUMULO_API_H

using namespace std;
using namespace accumulo;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace boost;

class Authorizations {
	
	set<string> *authsVector;
	
public:
	
	Authorizations(const string &auths);
	~Authorizations();
	set<string> *getAuthorizations() const;
};

class Mutation {
	
	string rowId;
	vector<ColumnUpdate> *updates;

public:
	
	Mutation(const string& rowId);
	~Mutation();
	void put(const string& colFam, const string& colQual, const string& colVis, const int64_t timestamp, const string& value);
	string getRowId() const;
	const vector<ColumnUpdate> *getUpdates() const;
	void clear();
};

class BatchWriter {

	shared_ptr<AccumuloProxyClient> client;
	string login;
	string writerToken;
	string tableName;
		
public:
	
	BatchWriter(shared_ptr<AccumuloProxyClient> proxyClient, const string &login, const string &tableName,
	       const int64_t maxMemory, const int64_t latencyMs, const int64_t timeoutMs, const int32_t numThreads);
	~BatchWriter();
	void addMutation(Mutation &mutation);
	void flush(void);
	void close(void); 
};

class BatchScannerIterator {

	shared_ptr<AccumuloProxyClient> client;
	BatchScanOptions options;
	string login;
	string scannerToken;
	string tableName;

public:
	BatchScannerIterator(shared_ptr<AccumuloProxyClient> proxyClient, const string& login, const string& tableName, BatchScanOptions options); 
	bool hasNext(void);
	KeyValue next(void);
	void close();
};

// TODO: Combine Scanner & BatchScanner into implementations of a class with pluggable options
class BatchScanner {
	
		shared_ptr<AccumuloProxyClient> client;

		vector<ScanColumn> *columns;
		vector<IteratorSetting> *iterators;

		BatchScanOptions *options;
		string login;
		string tableName;

	public:
		BatchScanner(shared_ptr<AccumuloProxyClient> client, const string& login, const string& tableName, 
				const Authorizations &authorizations, int32_t numThreads);
		BatchScannerIterator iterator(void);
		void setRanges(const vector<Range> &ranges);
		void fetchColumn(const string& colFam, const string& colQual);
		void fetchColumnFamily(const string &colFam);
		void attachScanIterator(const IteratorSetting &iteratorSetting);
};

class ScannerIterator {
	
	shared_ptr<AccumuloProxyClient> client;
	ScanOptions options;
	string login;
	string scannerToken;
	string tableName;
	
public:
	ScannerIterator(shared_ptr<AccumuloProxyClient> proxyClient, const string& login, const string& tableName, ScanOptions options); 
	bool hasNext(void);
	KeyValue next(void);
	void close();
	
};

class Scanner {
	
	shared_ptr<AccumuloProxyClient> client;
	
	vector<ScanColumn> columns;
	vector<IteratorSetting> iterators;

	ScanOptions options;
	string login;
	string tableName;
	
public:
	Scanner(shared_ptr<AccumuloProxyClient> client, const string& login, const string& tableName, 
			const Authorizations &authorizations);
	ScannerIterator iterator(void);
	void setRange(const Range &range);
	void setRange(Range *range);
	void fetchColumn(const string& colFam, const string& colQual);
	void fetchColumnFamily(const string &colFam);
	void attachScanIterator(const IteratorSetting &iteratorSetting);
};


class Connector {
	
	string login;
	
	shared_ptr<TSocket> socket;
	shared_ptr<TTransport> transport;
	shared_ptr<TProtocol> protocol;

	shared_ptr<AccumuloProxyClient> client;
	
public:

	Connector(const string& host, int port, const string& username, const string& password);
	void createTable(const string& tableName);
	BatchWriter createBatchWriter(const string& tableName, int64_t maxMemory, int64_t latencyMs, int64_t timeoutMs, int32_t numThreads);
	Scanner createScanner(const string& tableName, const Authorizations &authorizations);
	BatchScanner createBatchScanner(const string& tableName, const Authorizations &authorizations, const int32_t numThreads);
	void close();
	
};

#endif
