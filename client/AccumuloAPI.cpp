#include "AccumuloAPI.h"

Authorizations::Authorizations(const string& auths) {

    string::size_type lastPos = auths.find_first_not_of(",", 0);
    string::size_type pos = auths.find_first_of(",", lastPos);

    while (string::npos != pos || string::npos != lastPos) {
    	authsVector.insert(auths.substr(lastPos, pos - lastPos));
    	lastPos = auths.find_first_not_of(",", pos);
    	pos = auths.find_first_of(",", lastPos);
    }
}

const set<string> Authorizations::getAuthorizations() const {
	
    return authsVector;
}

Authorizations::~Authorizations() {
}

Mutation::Mutation(const string &rowId) {
    this->rowId = rowId;
}

Mutation::~Mutation() {
}

void Mutation::put(const string &colFam, const string& colQual, const string& colVis, const int64_t timestamp, const string& value) {

    ColumnUpdate cUpdate;
    cUpdate.__set_colFamily(colFam);
    cUpdate.__set_colQualifier(colQual);
    cUpdate.__set_colVisibility(colVis);
    cUpdate.__set_timestamp(timestamp);
    cUpdate.__set_value(value);
	
    updates.push_back(cUpdate);
}

string Mutation::getRowId() const {
    return rowId;
}

const vector<ColumnUpdate> Mutation::getUpdates() const {
    return updates;
}

void Mutation::clear() {
	
    updates.clear();
}


BatchWriter::BatchWriter(boost::shared_ptr<AccumuloProxyClient> proxyClient, const string &login, const string &tableName,
			 const int64_t maxMemory, const int64_t latencyMs, const int64_t timeoutMs, const int32_t numThreads) {
	
    WriterOptions writerOptions;
    writerOptions.__set_maxMemory(maxMemory);
    writerOptions.__set_latencyMs(latencyMs);
    writerOptions.__set_timeoutMs(timeoutMs);
    writerOptions.__set_threads(numThreads);

    this->client = proxyClient;
	
    this->tableName = tableName;
    this->login = login;
	
    void createWriter(std::string& _return, const std::string& login, const std::string& tableName, const WriterOptions &opts);
  
    client.get()->createWriter(writerToken, login, tableName, writerOptions);
}

BatchWriter::~BatchWriter() {
}

void BatchWriter::addMutation(Mutation &mutation) {
	
    map<string, vector<ColumnUpdate> > cells;

    const vector<ColumnUpdate> updates = mutation.getUpdates();
    const string rowId = mutation.getRowId();

    cells.insert(make_pair(rowId, updates));

    client.get()->update(writerToken, cells);
	
    mutation.clear();
}

void BatchWriter::flush() {
	
    client->flush(writerToken);
}

void BatchWriter::close() {
    client->closeWriter(writerToken);
}

BatchScannerIterator::BatchScannerIterator(boost::shared_ptr<AccumuloProxyClient> proxyClient, const string &login, const string &tableName, 
					   BatchScanOptions &options) {
					
    this->client = proxyClient;
    this->options = options;
    this->tableName = tableName;
    this->login = login;
	
    client.get()->createBatchScanner(scannerToken, login, tableName, options);
}

bool BatchScannerIterator::hasNext() {
	
    return client.get()->hasNext(scannerToken);
}

const KeyValue BatchScannerIterator::next() {

    KeyValueAndPeek kvap;
    KeyValue kv;
	
    if(hasNext()) {
	client.get()->nextEntry(kvap, scannerToken);
	kv = kvap.keyValue;
    }

    return kv;
}

void BatchScannerIterator::close() {
    client.get()->closeScanner(scannerToken);
}

BatchScanner::BatchScanner(boost::shared_ptr<AccumuloProxyClient> proxyClient, const string& login, const string& tableName, 
			   const Authorizations &authorizations, const int32_t numThreads) {

    set<string> auths = authorizations.getAuthorizations();

    options.__set_authorizations(auths);
    options.__set_threads(numThreads);
    options.__set_columns(columns);
    options.__set_iterators(iterators);

    this->client = proxyClient;
    this->login = login;
    this->tableName = tableName;
}

void BatchScanner::setRanges(const vector<Range> &ranges) {
    options.__set_ranges(ranges);
}

BatchScannerIterator BatchScanner::iterator(void) {
	
    options.__set_columns(columns);
    options.__set_iterators(iterators);
	
    BatchScannerIterator iterator(client, login, tableName, options);
    return iterator;
}

void BatchScanner::fetchColumn(const string& colFamily, const string& colQual) {

    ScanColumn scanColumn;
    scanColumn.__set_colFamily(colFamily);
    scanColumn.__set_colQualifier(colQual);
	
    columns.push_back(scanColumn);
}

void BatchScanner::fetchColumnFamily(const string& colFamily) {

    ScanColumn scanColumn;
    scanColumn.__set_colFamily(colFamily);
	
    columns.push_back(scanColumn);
}

void BatchScanner::attachScanIterator(const IteratorSetting &iteratorSetting) {
	
    iterators.push_back(iteratorSetting);
}

ScannerIterator::ScannerIterator(boost::shared_ptr<AccumuloProxyClient> proxyClient, const string& login, const string& tableName, 
				 ScanOptions &options) {
					
    this->client = proxyClient;
    this->options = options;
    this->tableName = tableName;
    this->login = login;

    client.get()->createScanner(scannerToken, login, tableName, options);
}

bool ScannerIterator::hasNext() {
	
    return client.get()->hasNext(scannerToken);
}

const KeyValue ScannerIterator::next() {

    KeyValueAndPeek kvap;
    KeyValue kv;
	
    if(hasNext()) {
	client.get()->nextEntry(kvap, scannerToken);
	kv = kvap.keyValue;
    }

    return kv;
}

void ScannerIterator::close() {
    client.get()->closeScanner(scannerToken);
}

Scanner::Scanner(boost::shared_ptr<AccumuloProxyClient> proxyClient, const string& login, const string& tableName, 
		 const Authorizations &authorizations) {

    set<string> auths = authorizations.getAuthorizations();

    options.__set_authorizations(auths);
    options.__set_bufferSize(500000);
    options.__set_columns(columns);
    options.__set_iterators(iterators);

    this->client = proxyClient;
    this->login = login;
    this->tableName = tableName;
}

void Scanner::setRange(const Range &range) {
    options.__set_range(range);
}

ScannerIterator Scanner::iterator(void) {
	
    options.__set_columns(columns);
    options.__set_iterators(iterators);
	
    ScannerIterator iterator(client, login, tableName, options);
    return iterator;
}

void Scanner::fetchColumn(const string& colFamily, const string& colQual) {

    ScanColumn scanColumn;
    scanColumn.__set_colFamily(colFamily);
    scanColumn.__set_colQualifier(colQual);
	
    columns.push_back(scanColumn);
}

void Scanner::fetchColumnFamily(const string& colFamily) {

    ScanColumn scanColumn;
    scanColumn.__set_colFamily(colFamily);
	
    columns.push_back(scanColumn);
}

void Scanner::attachScanIterator(const IteratorSetting &iteratorSetting) {
	
    iterators.push_back(iteratorSetting);
}

Connector::Connector(const string& host, int port, const string& username, const string& password) {

    boost::shared_ptr<TSocket> newSocket(new TSocket(host, port));
    boost::shared_ptr<TTransport> newTransport(new TFramedTransport(newSocket));
    boost::shared_ptr<TProtocol> newProtocol(new TCompactProtocol(newTransport));

    socket = newSocket;
    transport = newTransport;
    protocol = newProtocol;

    transport->open();

    string passkey("password");

    map<string, string> m;
    m.insert(make_pair(passkey, password));
	
    client.reset(new AccumuloProxyClient(protocol));
	
    client.get()->login(login, username, m);

    _tableOperations = new TableOperations(client, login);
}

void Connector::close() {
    transport->close();
}

BatchWriter Connector::createBatchWriter(const string& tableName, const int64_t maxMemory, 
					 const int64_t latencyMs, const int64_t timeoutMs, const int32_t numThreads) {
	
    BatchWriter writer(client, login, tableName, maxMemory, latencyMs, timeoutMs, numThreads);
    return writer;
}

Scanner Connector::createScanner(const string& tableName, const Authorizations &authorizations) {
	
    Scanner scanner(client, login, tableName, authorizations);
    return scanner;
}

BatchScanner Connector::createBatchScanner(const string &tableName, const Authorizations &authorizations, const int32_t numThreads) {
	
    BatchScanner scanner(client, login, tableName, authorizations, numThreads);
    return scanner;
}

TableOperations Connector::tableOperations() {
    return *_tableOperations;
}
