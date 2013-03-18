#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

	if(argc < 7 || argc > 9) {
		cout << "Usage: " << argv[0] << " <host> <port> <startRowId1> <stopRowId1> <startRowId2> <stopRowId2> <colFam> <colQual>\n";	
		return 1;
	}
	
	Connector connector(argv[1], atoi(argv[2]), "root", "secret");

	set<string> auths;
	auths.insert("U");
	
	BatchScanner scanner = connector.createBatchScanner("testTable", auths, 5);
	
	// construct ranges
	Range range1(new Key(argv[3]), new Key(argv[4]));
	Range range2(new Key(argv[5]), new Key(argv[6]));
	
	vector<Range> ranges;
	ranges.push_back(range1);
	ranges.push_back(range2);
	
	scanner.setRanges(ranges);
	
	if(argc > 7) {
		
		if(argc == 8) {
			
			scanner.fetchColumnFamily(argv[7]);
		}
		
		else if(argc == 9) {

			scanner.fetchColumn(argv[7], argv[8]);
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
