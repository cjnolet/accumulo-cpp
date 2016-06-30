Accumulo++
==========

A C++ library for Apache Accumulo

## Why a C++ Library for Accumulo?

Simple- When I'm running logic in InfoSphere Streams, I'd like to talk to Accumulo in C/C++. If I'm running code that needs to utilize CUDA and the GPU, I'd like to stay in C/C++ also. In Accumulo 1.5, we were given an Apache Thrift-based proxy that allows us to generate clients in differing languages. This is one such client which has been augmented to look as close as possible to the Java-based API that we've all come to know and love.

Original version by cjnoleh, ported to Accumulo 2.7.2 by cybermaggedon.

## Requirements

1. A running Accumulo cluster
2. The new Accumulo Thrift Proxy server
3. Thrift C++ library installed (http://thrift.apache.org/docs/install/)

## Quick Start

1. You'll need to run the Accumulo Thrift Proxy server.
```
${ACCUMULO_HOME}/bin/accumulo proxy -p ${SOME_PLACE}/proxy.properties
```
2. Build the example programs:
```
make
```
3. Create an example table using the CreateTable example program
```
./target/CreateTableExample localhost 42424 root secret testTable
```
4. Run the BatchWriter example program to insert some rows into Accumulo:
```
./target/BatchWriterExample localhost 42424 root secret testTable row1 col1 qual6 U val
```
5. Run the Scanner example program to read rows from Accumulo:
```
./target/ScannerExample localhost 42424 root secret testTable A z
```

## Code Samples

### TableOperations Sample
```c++

try {
	Conector connector("localhost", 42424, "root", "secret");
	connector.tableOperations().createTable("testTable");
	connector.tableOperations().compactTable("testTable");
	connector.close();

} catch(AccumuloSecurityException &e) {
	cout << "There was a problem with the given credentials.\n";

} catch(TableNotFoundException &e) {
	cout << "The specified table was not found.\n";
}

```

### BatchWriter Sample
```c++

try {
	Connector connector("localhost", 42424, "root", "secret");
	BatchWriter writer = connector.createBatchWriter("testTable", 500000, 10000, 10000, 2);

	Mutation mutation("rowId");
	mutation.put("colFam", "colQual", "colVis", 500000, "val");

	writer.addMutation(mutation);
	writer.flush();
	writer.close();
	
	connector.close();

} catch(AccumuloSecurityException &e) {
	cout << "There was a problem with the given credentials.\n";

} catch(TException &e) {
	cout << "An error occurred: the mutation was not written to the table.\n";
}
```  

### Scanner Sample

```c++
	
    try {
		
	Connector connector("localhost", 42424, "user", "password");

	Authorizations auths("");
	Scanner scanner = connector.createScanner("mytable", auths);

	Key ks, ke;
	ks.row = "banana"; ke.row = "chaffinch";

	Range range;
	range.start = ks;
	range.stop = ke;

	scanner.setRange(range);
	scanner.fetchColumnFamily("wingspan");

	ScannerIterator itr = scanner.iterator();

	while(itr.hasNext()) {
	    KeyValue kv = itr.next();

	    cout << kv.key.row << " " << kv.key.colFamily << ":" 
		 << kv.key.colQualifier << " [" << kv.key.colVisibility
		 << "] " << kv.key.timestamp << "\t" << kv.value << "\n";

	}

	itr.close();
	connector.close();

    } catch(TableNotFoundException &e) {
	cout << "The specified table was not found\n";
    }

```

### BatchScanner Example

```c++

    Connector connector("localhost", 42424, "user", "password");

    Authorizations auths("");
    BatchScanner scanner = connector.createBatchScanner("mytable", auths, 5);
	
    // construct ranges
    Key r1s; r1s.row = "abc1";
    Key r1e; r1e.row = "def1";
    Key r2s; r2s.row = "acd1";
    Key r2e; r2e.row = "dhi3";
    Range range1;
    range1.start = r1s;
    range1.stop = r1e;
    Range range2;
    range2.start = r2s;
    range2.stop = r2e;
	
    vector<Range> ranges;
    ranges.push_back(range1);
    ranges.push_back(range2);
	
    scanner.setRanges(ranges);
    scanner.fetchColumnFamily("columnf");
	
    BatchScannerIterator itr = scanner.iterator();
	
    while(itr.hasNext()) {
	KeyValue kv = itr.next();

	cout << kv.key.row << " " << kv.key.colFamily << ":" 
	     << kv.key.colQualifier << " [" << kv.key.colVisibility 
	     << "] " << kv.key.timestamp << "\t" << kv.value << "\n";

    }
	
    itr.close();
    connector.close();

```

