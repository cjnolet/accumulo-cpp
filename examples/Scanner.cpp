#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {


	if(argc < 8 || argc > 10) {
		cout << "Usage: " << argv[0] << " <host> <port> <username> <password> <tableName> <startRowId> <stopRowId> <colFam> <colQual>\n";	
		return 1;
	}
	
	try {
		
		Connector connector(argv[1], atoi(argv[2]), argv[3], argv[4]);

		Authorizations auths("");
		Scanner scanner = connector.createScanner(argv[5], auths);

		Key ks, ke;
		ks.row = argv[6]; ke.row = argv[7];

		Range range;
		range.start = ks;
		range.stop = ke;

		scanner.setRange(range);

		if(argc > 8) {

			if(argc == 9) {

				scanner.fetchColumnFamily(argv[8]);
			}

			else if(argc == 10) {

				scanner.fetchColumn(argv[8], argv[9]);
			}
		}

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
	
	return 0;
}
