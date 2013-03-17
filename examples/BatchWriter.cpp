#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

	if(argc != 8) {
		cout << "Usage: " << argv[0] << " <host> <port> <rowId> <colFam> <colQual> <colVis> <val>\n";	
		return 1;
	}

	
	Connector connector(argv[1], atoi(argv[2]), string("root"), string("secret"));
	BatchWriter writer = connector.createBatchWriter(string("testTable"), 500, 100, 100, 1);

	string rowId(argv[3]);
	string colFam(argv[4]);
	string colQual(argv[5]);
	string colVis(argv[6]);
	string val(argv[7]);

	Mutation mutation(rowId);
	mutation.put(colFam, colQual, colVis, int64_t(500000), val);

	writer.addMutation(mutation);
	writer.flush();
	writer.close();
	
	connector.close();
	
	return 0;
}
