#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

    if(argc != 6) {
	cout << "Usage: " << argv[0] << " <host> <port> <username> <password> <tableName>\n";	
	return 1;
    }
	
    try {
		
	Connector connector(argv[1], atoi(argv[2]), argv[3], argv[4]);
	connector.tableOperations().createTable(argv[5]);
	connector.close();

	return 0;
    } catch(AccumuloSecurityException &e) {
	cout << "There was a problem with your credentials\n";
    }
}
