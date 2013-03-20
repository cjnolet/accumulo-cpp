#include <stdlib.h>
#include <boost/shared_ptr.hpp>
#include "../proxy/AccumuloProxy.h"

#ifndef TABLE_OPERATIONS_H
#define TABLE_OPERATIONS_H

using namespace accumulo;
using namespace boost;
using namespace std;

class TableOperations {
	
	shared_ptr<AccumuloProxyClient> client;
	string login;
	
public:
	TableOperations(shared_ptr<AccumuloProxyClient> client, const string &login);
	void createTable(const string& tableName);
};

#endif