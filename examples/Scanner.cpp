#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {


	if(argc < 8 || argc > 10) {
		cout << "Usage: " << argv[0] << " <host> <port> <username> <password> <tableName> <startRowId> <stopRowId> <colFam> <colQual>\n";	
		return 1;
	}
	
	Connector connector(argv[1], atoi(argv[2]), argv[3], argv[4]);

	Authorizations auths("A,B");
	Scanner scanner = connector.createScanner(argv[5], auths);
	scanner.setRange(new Range(new Key(argv[6]), new Key(argv[7])));
	
	if(argc > 8) {
		
		string colFam(argv[8]);
		
		if(argc == 9) {
			
			scanner.fetchColumnFamily(colFam);
		}
		
		else if(argc == 10) {
			
			string colQual(argv[9]);
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
