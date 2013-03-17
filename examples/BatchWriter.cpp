#include "../client/AccumuloAPI.h"

int main(int argc, char* argv[]) {

	if(argc != 4) {
		cout << "Usage: " << argv[0] << " <host> <port> <numMutations>\n";	
		return 1;
	}

	
	Connector connector(argv[1], atoi(argv[2]), string("root"), string("secret"));
	BatchWriter writer = connector.createBatchWriter(string("testTable"), 500, 100, 100, 1);

	Mutation mutation(string("rowid1"));
	
	int numItems = atoi(argv[3]);
	
	for(int i = 0; i < numItems; i++) {
		
		mutation.put( string("colFam"), string("colQual"), string("colVis"), int64_t(500000), string("val"));

		if(i > 0 && i % 10000 == 0) {
			
			writer.addMutation(mutation);
			cout << "Written " << i << " out of " << numItems << " so far...\n";
		}
	}
	
	writer.addMutation(mutation);
	writer.flush();
	writer.close();
	
	connector.close();
	
	cout << "\nSuccessfully wrote " << numItems << " mutations.\n";

	return 0;
}
