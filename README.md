Accumulo++
==========

A C++ library for Apache Accumulo

## Why a C++ Library for Accumulo?

Simple- When I'm running logic in InfoSphere Streams, I'd like to talk to Accumulo in C/C++. If I'm running code that needs to utilize CUDA and the GPU, I'd like to stay in C/C++ also. In Accumulo 1.5, we were given an Apache Thrift-based proxy that allows us to generate clients in differing languages. This is one such client which has been augmented to look as close as possible to the Java-based API that we've all come to know and love.

## Requirements

1. A running Accumulo cluster
2. The new Accumulo Thrift Proxy server (1.6 included in this codebase)
3. Thrift C++ library installed (http://thrift.apache.org/docs/install/)

## Code Samples

### BatchWriter Sample
```c++
Connector connector("localhost", 42424, "root", "secret");
BatchWriter writer = connector.createBatchWriter("testTable", 500, 100, 100, 1);

Mutation mutation("rowId");
mutation.put("colFam", "colQual", "colVis", int64_t(500000), "val");

writer.addMutation(mutation);
writer.flush();
writer.close();
	
connector.close();
```  

### Scanner Sample

```c++
Connector connector("localhost", 42424, "root", "secret");

set<string> auths;
auths.insert(string("U"));
	
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
```
