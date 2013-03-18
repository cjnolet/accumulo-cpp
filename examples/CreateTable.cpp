#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

	if(argc != 6) {
		cout << "Usage: " << argv[0] << " <host> <port> <username> <password> <tableName>\n";	
		return 1;
	}
	
	Connector connector(argv[1], atoi(argv[2]), argv[3], argv[4]);

	connector.createTable(argv[5]);
	connector.close();
	
	return 0;
}
