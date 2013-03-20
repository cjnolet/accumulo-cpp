Accumulo++
==========

A C++ library for Apache Accumulo

## Why a C++ Library for Accumulo?

Simple- When I'm running logic in InfoSphere Streams, I'd like to talk to Accumulo in C/C++. If I'm running code that needs to utilize CUDA and the GPU, I'd like to stay in C/C++ also. In Accumulo 1.5, we were given an Apache Thrift-based proxy that allows us to generate clients in differing languages. This is one such client which has been augmented to look as close as possible to the Java-based API that we've all come to know and love.

## Requirements

1. A running Accumulo cluster
2. The new Accumulo Thrift Proxy server (1.6 included in this codebase)
3. Thrift C++ library installed (http://thrift.apache.org/docs/install/)

## Quick Start

1. You'll need to run the Accumulo Thrift Proxy server. A jar is packaged with this codebase in order to get this started quickly. Locate the server/proxy.properties file and fill in the necessary fields. When you are done, start up the server by running the following:
```
./server/runProxy.sh
```
2. Build the example programs:
```
make
```
3. Create an example table using the CreateTable example program
```
./target/CreateTableExample.o localhost 42424 root secret testTable
```
4. Run the BatchWriter example program to insert some rows into Accumulo:
```
./target/BatchWriterExample.o localhost 42424 root secret testTable row1 col1 qual6 U val
```
5. Run the Scanner example program to read rows from Accumulo:
```
./target/ScannerExample.o localhost 42424 root secret testTable A z
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
	Connector connector("localhost", 42424, "root", "secret");

	Authorizations auths("A,B");
	Scanner scanner = connector.createScanner("testTable", auths);

	// Set up the range
	Key start("A");
	Key stop("z");
	Range range(start, stop);

	scanner.setRange(range);
	scanner.fetchColumn("department", "1");

	ScannerIterator itr = scanner.iterator();
	
	while(itr.hasNext()) {
		KeyValue kv = itr.next();

		cout << kv.getKey().getRow() << " " << kv.getKey().getColFamily() << ":" 
				 << kv.getKey().getColQualifier() << " [" << kv.getKey().getColVisibility() 
				 << "] " << kv.getKey().getTimestamp() << "\t" << kv.getValue() << "\n";

	}
	
	itr.close();
	connector.close();

} catch(AccumuloSecurityException &e) {
	cout << "There was a problem with the given credentials.\n";

} catch(TableNotFoundException &e) {
	cout << "The specified table was not found.\n";
}

```

### BatchScanner Example

```c++

try {
	Connector connector("localhost", 42424, "root", "secret");

	Authorizations auths("A,B");	
	BatchScanner scanner = connector.createBatchScanner("testTable", auths, 5);
	
	// construct ranges
	Range range1(new Key("A"), new Key("Z");
	Range range2(new Key("a"), new Key("z");
	
	vector<Range> ranges;
	ranges.push_back(range1);
	ranges.push_back(range2);
	
	scanner.setRanges(ranges);
	scanner.fetchColumn("department", "1");
	
	BatchScannerIterator itr = scanner.iterator();
	
	while(itr.hasNext()) {

		KeyValue kv = itr.next();

		cout << kv.getKey().getRow() << " " << kv.getKey().getColFamily() << ":" 
			 << kv.getKey().getColQualifier() << " [" << kv.getKey().getColVisibility() 
			 << "] " << kv.getKey().getTimestamp() << "\t" << kv.getValue() << "\n";

	}
	
	itr.close();
	connector.close();
	
} catch(AccumuloSecurityException &e) {
	cout << "There was a problem with the given credentials.\n";

} catch(TableNotFoundException &e) {
	cout << "The specified table was not found.\n";
}
```

