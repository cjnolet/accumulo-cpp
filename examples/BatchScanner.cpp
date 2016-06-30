#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

	if(argc < 10 || argc > 12) {
		cout << "Usage: " << argv[0] << " <host> <port> <username> <password> <testTable> <startRowId1> <stopRowId1> <startRowId2> <stopRowId2> <colFam> <colQual>\n";	
		return 1;
	}
	
	Connector connector(argv[1], atoi(argv[2]), argv[3], argv[4]);

	Authorizations auths("");
	BatchScanner scanner = connector.createBatchScanner(argv[5], auths, 5);
	
	// construct ranges
	Key r1s; r1s.row = argv[6];
	Key r1e; r1e.row = argv[7];
	Key r2s; r2s.row = argv[8];
	Key r2e; r2e.row = argv[9];
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

		cout << kv.key.row << " " << kv.key.colFamily << ":" 
			 << kv.key.colQualifier << " [" << kv.key.colVisibility 
			 << "] " << kv.key.timestamp << "\t" << kv.value << "\n";

	}
	
	itr.close();
	connector.close();
	
	return 0;
}
