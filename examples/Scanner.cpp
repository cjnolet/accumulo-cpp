#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

	if(argc < 5 || argc > 7) {
		cout << "Usage: " << argv[0] << " <host> <port> <startRowId> <stopRowId> <colFam> <colQual>\n";	
		return 1;
	}
	
	Connector connector(argv[1], atoi(argv[2]), string("root"), string("secret"));

	set<string> auths;
	auths.insert(string("U"));
	
	Scanner scanner = connector.createScanner(string("testTable"), auths);

	Key start(argv[3]);
	Key stop(argv[4]);
	Range range(start, stop);
	scanner.setRange(range);
	
	if(argc > 5) {
		
		string colFam(argv[5]);
		
		if(argc == 6) {
			
			scanner.fetchColumnFamily(colFam);
		}
		
		else if(argc == 7) {
			
			string colQual(argv[6]);
			scanner.fetchColumn(colFam, colQual);
		}
	}
	
	ScannerIterator itr = scanner.iterator();
	
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
