#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

	if(argc != 8) {
		cout << "Usage: " << argv[0] << " <host> <port> <rowId> <colFam> <colQual> <colVis> <val>\n";	
		return 1;
	}

	Connector connector(argv[1], atoi(argv[2]), "root", "secret");
	BatchWriter writer = connector.createBatchWriter("testTable", 500, 100, 100, 1);

	Mutation mutation(argv[3]);
	mutation.put(argv[4], argv[5], argv[6], 500000, argv[7]);

	writer.addMutation(mutation);
	writer.flush();
	writer.close();
	
	connector.close();
	
	return 0;
}
