#include <ctime>
#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

    if(argc != 11) {
	cout << "Usage: " << argv[0] << " <host> <port> <username> <password> <tableName> <rowId> <colFam> <colQual> <colVis> <val>\n";	
	return 1;
    }

    Connector connector(argv[1], atoi(argv[2]), argv[3], argv[4]);
    BatchWriter writer = connector.createBatchWriter(argv[5], 5000, 10000, 10000, 2);

		
    const long double sysTime = time(0)*1000;


    Mutation mutation(argv[6]);
    mutation.put(argv[7], argv[8], argv[9], sysTime, argv[10]);

    writer.addMutation(mutation);
    writer.flush();
    writer.close();
	
    connector.close();
	
    return 0;
}
