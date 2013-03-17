#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

	if(argc != 5) {
		cout << "Usage: " << argv[0] << " <host> <port> <startRowId> <stopRowId>\n";	
		return 1;
	}

	
	Connector connector(argv[1], atoi(argv[2]), string("root"), string("secret"));

	set<string> auths;
	auths.insert(string("U"));
	
	Scanner scanner = connector.createScanner(string("testTable"), auths);

	Range range;
	Key key;
	key.__set_row(string(argv[3]));
	range.__set_start(key);
	
	Key stopKey;
	stopKey.__set_row(string(argv[4]));
	range.__set_stop(stopKey);

	scanner.setRange(range);
	
	ScannerIterator itr = scanner.iterator();
	
	while(itr.hasNext()) {
		KeyValue kv = itr.next();

		cout << "ROW:\t" + kv.getKey().getRow() << "\t" << kv.getKey().getColFamily() << ":" << kv.getKey().getColQualifier() << "\t[" <<
				kv.getKey().getColVisibility() << "]\t" << kv.getKey().getTimestamp() << "\t\t" << kv.getValue() << "\n";

	}
	
	connector.close();
	
	return 0;
}
