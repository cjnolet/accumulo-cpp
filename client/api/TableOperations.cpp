#include "TableOperations.h"


TableOperations::TableOperations(shared_ptr<AccumuloProxyClient> client, const string &login) {
	
	this->login = login;
	this->client = client;
}

void TableOperations::createTable(const string& tableName) {
	client->createTable(login, tableName, true, TimeType::MILLIS);
}
