#include "TableOperations.h"


TableOperations::TableOperations(boost::shared_ptr<AccumuloProxyClient> client, const string &login) {
	
	this->login = login;
	this->client = client;
}

void TableOperations::createTable(const string& tableName) {
	client->createTable(login, tableName, true, TimeType::MILLIS);
}

void TableOperations::addSplits(const string &tableName, const set<string> &splits) {
	
	client->addSplits(login, tableName, splits);
}

void TableOperations::compactTable(string &tableName, string &startRow, string &endRow, const vector<IteratorSetting> &iterators, 
								   const bool flush, const bool wait) {
									
	client->compactTable(login, tableName, startRow, endRow, iterators, flush, wait);
}

void TableOperations::cancelCompaction(const std::string& tableName) {
	client->cancelCompaction(login, tableName);
}

