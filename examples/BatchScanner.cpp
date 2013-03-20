#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

	if(argc < 10 || argc > 12) {
		cout << "Usage: " << argv[0] << " <host> <port> <username> <password> <testTable> <startRowId1> <stopRowId1> <startRowId2> <stopRowId2> <colFam> <colQual>\n";	
		return 1;
	}
	
	Connector connector(argv[1], atoi(argv[2]), argv[3], argv[4]);

	Authorizations auths("A,B");
	BatchScanner scanner = connector.createBatchScanner(argv[5], auths, 5);
	
	// construct ranges
	Range range1(new Key(argv[6]), new Key(argv[7]));
	Range range2(new Key(argv[8]), new Key(argv[9]));
	
	vector<Range> ranges;
	ranges.push_back(range1);
	ranges.push_back(range2);
	
	scanner.setRanges(ranges);
	
	if(argc > 10) {
		
		if(argc == 11) {
			
			scanner.fetchColumnFamily(argv[10]);
		}
		
		else if(argc == 12) {

			scanner.fetchColumn(argv[10], argv[11]);
		}
	}
	
	BatchScannerIterator itr = scanner.iterator();
	
	while(itr.hasNext()) {
		KeyValue kv = itr.next();

		cout << kv.getKey().getRow() << " " << kv.getKey().getColFamily() << ":" 
			 << kv.getKey().getColQualifier() << " [" << kv.getKey().getColVisibility() 
			 << "] " << kv.getKey().getTimestamp() << "\t" << kv.getValue() << "\n";

	}
	
	itr.close();
	connector.close();
	
	return 0;
}
