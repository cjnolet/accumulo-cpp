#include <stdlib.h>
#include <boost/shared_ptr.hpp>
#include "AccumuloProxy.h"

#ifndef TABLE_OPERATIONS_H
#define TABLE_OPERATIONS_H

using namespace accumulo;
using namespace boost;
using namespace std;

class TableOperations {
	
    boost::shared_ptr<AccumuloProxyClient> client;
    string login;
	
public:
    TableOperations(boost::shared_ptr<AccumuloProxyClient> client, const string &login);
    void createTable(const string& tableName);
    void addSplits(const string &tableName, const set<string> &splits);
    void compactTable(string &tableName, string &startRow, string &endRow, const vector<IteratorSetting> &iterators, 
		      const bool flush, const bool wait);
    void cancelCompaction(const string& tableName);
};

#endif
