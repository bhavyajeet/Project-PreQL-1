#include "global.h"

/**
 * @brief Construct a new Table:: Table object
 *
 */
Table::Table()
{
    logger.log("Table::Table");
}

/**
 * @brief Construct a new Table:: Table object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param tableName 
 */
Table::Table(string tableName)
{
    logger.log("Table::Table");
    this->sourceFileName = "../data/" + tableName + ".csv";
    this->tableName = tableName;
}

/**
 * @brief Construct a new Table:: Table object used when an assignment command
 * is encountered. To create the table object both the table name and the
 * columns the table holds should be specified.
 *
 * @param tableName 
 * @param columns 
 */
Table::Table(string tableName, vector<string> columns)
{
    logger.log("Table::Table");
    this->sourceFileName = "../data/temp/" + tableName + ".csv";
    this->tableName = tableName;
    this->columns = columns;
    this->columnCount = columns.size();
    this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (32 * columnCount));
    this->writeRow<string>(columns);
}

/**
 * @brief The load function is used when the LOAD command is encountered. It
 * reads data from the source file, splits it into blocks and updates table
 * statistics.
 *
 * @return true if the table has been successfully loaded 
 * @return false if an error occurred 
 */
bool Table::load()
{
    logger.log("Table::load");
    logger.log(this->sourceFileName);
    fstream fin(this->sourceFileName, ios::in);
    string line;
    if (getline(fin, line))
    {
        fin.close();
        if (this->extractColumnNames(line))
            if (this->blockify())
                return true;
    }
    fin.close();
    return false;
}

/**
 * @brief Function extracts column names from the header line of the .csv data
 * file. 
 *
 * @param line 
 * @return true if column names successfully extracted (i.e. no column name
 * repeats)
 * @return false otherwise
 */
bool Table::extractColumnNames(string firstLine)
{
    logger.log("Table::extractColumnNames");
    unordered_set<string> columnNames;
    string word;
    stringstream s(firstLine);
    while (getline(s, word, ','))
    {
        word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
        if (columnNames.count(word))
            return false;
        columnNames.insert(word);
        this->columns.emplace_back(word);
    }
    this->columnCount = this->columns.size();
    this->maxRowsPerBlock = (uint)((BLOCK_SIZE * 1000) / (32 * this->columnCount));
    return true;
}

/**
 * @brief This function splits all the rows and stores them in multiple files of
 * one block size. 
 *
 * @return true if successfully blockified
 * @return false otherwise
 */
bool Table::blockify()
{
    logger.log("Table::blockify");
    ifstream fin(this->sourceFileName, ios::in);
    string line, word;
    vector<int> row(this->columnCount, 0);
    vector<vector<int>> rowsInPage(this->maxRowsPerBlock, row);
    int pageCounter = 0;
    unordered_set<int> dummy;
    dummy.clear();
    this->distinctValuesInColumns.assign(this->columnCount, dummy);
    this->columns.assign(this->columnCount,"");
    this->distinctValuesPerColumnCount.assign(this->columnCount, 0);
    getline(fin, line);
    stringstream s(line);
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (!getline(s, word, ','))
            return false;
        word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
        this->columns[columnCounter] = word;
        logger.log("ANNARA");
        logger.log(word);
        logger.log("OOPOPOP");
    }
    while (getline(fin, line))
    {
        stringstream s(line);
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (!getline(s, word, ','))
                return false;
            row[columnCounter] = stoi(word);
            rowsInPage[pageCounter][columnCounter] = row[columnCounter];
        }
        pageCounter++;
        this->updateStatistics(row);
        if (pageCounter == this->maxRowsPerBlock)
        {
            bufferManager.writePage(this->tableName, this->blockCount, rowsInPage, pageCounter);
            this->blockCount++;
            this->rowsPerBlockCount.emplace_back(pageCounter);
            pageCounter = 0;
        }
    }
    if (pageCounter)
    {
        bufferManager.writePage(this->tableName, this->blockCount, rowsInPage, pageCounter);
        this->blockCount++;
        this->rowsPerBlockCount.emplace_back(pageCounter);
        pageCounter = 0;
    }

    if (this->rowCount == 0)
        return false;
    this->distinctValuesInColumns.clear();
    return true;
}

/**
 * @brief Given a row of values, this function will update the statistics it
 * stores i.e. it updates the number of rows that are present in the column and
 * the number of distinct values present in each column. These statistics are to
 * be used during optimisation.
 *
 * @param row 
 */
void Table::updateStatistics(vector<int> row)
{
    this->rowCount++;
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (!this->distinctValuesInColumns[columnCounter].count(row[columnCounter]))
        {
            this->distinctValuesInColumns[columnCounter].insert(row[columnCounter]);
            this->distinctValuesPerColumnCount[columnCounter]++;
        }
    }
}

/**
 * @brief Checks if the given column is present in this table.
 *
 * @param columnName 
 * @return true 
 * @return false 
 */
bool Table::isColumn(string columnName)
{
    logger.log("Table::isColumn");
    cout << columnName;
    for (auto col : this->columns)
    {
        cout << "PPPPPPPPPPPPPPPPPPPp";
        cout << col;
        cout << endl;
        if (col == columnName)
        {
            cout << "OKOKOKOKOKOKO";
            return true;
        }
    }
    return false;
}

/**
 * @brief Renames the column indicated by fromColumnName to toColumnName. It is
 * assumed that checks such as the existence of fromColumnName and the non prior
 * existence of toColumnName are done.
 *
 * @param fromColumnName 
 * @param toColumnName 
 */
void Table::renameColumn(string fromColumnName, string toColumnName)
{
    logger.log("Table::renameColumn");
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (columns[columnCounter] == fromColumnName)
        {
            columns[columnCounter] = toColumnName;
            break;
        }
    }
    return;
}

/**
 * @brief Function prints the first few rows of the table. If the table contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Table::print()
{
    logger.log("Table::print");
    uint count = min((long long)PRINT_COUNT, this->rowCount);

    //print headings
    this->writeRow(this->columns, cout);
    cout << "LAUDA" << endl;
    cout << PRINT_COUNT;
    Cursor cursor(this->tableName, 0);
    vector<int> row;
    for (int rowCounter = 0; rowCounter < count; rowCounter++)
    {
        row = cursor.getNext();
        cout << row[0];
        cout << "ANANDkjesnfijrsngkjn";
        this->writeRow(row, cout);
    }
    printRowCount(this->rowCount);
}



/**
 * @brief This function returns one row of the table using the cursor object. It
 * returns an empty row is all rows have been read.
 *
 * @param cursor 
 * @return vector<int> 
 */
void Table::getNextPage(Cursor *cursor)
{
    logger.log("Table::getNext");

        if (cursor->pageIndex < this->blockCount - 1)
        {
            cursor->nextPage(cursor->pageIndex+1);
        }
}



/**
 * @brief called when EXPORT command is invoked to move source file to "data"
 * folder.
 *
 */
void Table::makePermanent()
{
    logger.log("Table::makePermanent");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->tableName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    //print headings
    this->writeRow(this->columns, fout);

    Cursor cursor(this->tableName, 0);
    vector<int> row;
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        row = cursor.getNext();
        this->writeRow(row, fout);
    }
    fout.close();
}

/**
 * @brief Function to check if table is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Table::isPermanent()
{
    logger.log("Table::isPermanent");
    if (this->sourceFileName == "../data/" + this->tableName + ".csv")
    return true;
    return false;
}

/**
 * @brief The unload function removes the table from the database by deleting
 * all temporary files created as part of this table
 *
 */
void Table::unload(){
    logger.log("Table::~unload");
    for (int pageCounter = 0; pageCounter < this->blockCount; pageCounter++)
        bufferManager.deleteFile(this->tableName, pageCounter);
    if (!isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
}

/**
 * @brief Function that returns a cursor that reads rows from this table
 * 
 * @return Cursor 
 */
Cursor Table::getCursor()
{
    logger.log("Table::getCursor");
    Cursor cursor(this->tableName, 0);
    return cursor;
}

/**
 * @brief Function that returns the index of column indicated by columnName
 * 
 * @param columnName 
 * @return int 
 */
int Table::getColumnIndex(string columnName)
{
    logger.log("Table::getColumnIndex");
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (this->columns[columnCounter] == columnName)
            return columnCounter;
    }
}

/**
 * @brief Function that returns the index of column indicated by indexName
 * 
 * @param  indexingStrategy
 * @return int 
 */
int Table::indexTable(string columnName,IndexingStrategy indexingStrategy, string thirdParam)
{
    logger.log("Index::creatingIndex");
    if(indexingStrategy == NOTHING){
        this->indexed = false;
        this->indexedColumn = "";
    }
    else{
        this->indexed = true;
        this->indexedColumn = columnName;
        for (int i = 0; i < this->columnCount; i++)
        {
            if(this->columns[i] == this->indexedColumn){
                this->indexedColumnNumber = i;
            }
        }
        if(indexingStrategy == BTREE){
            // construct a btree
            // thirdParam -> fanout
            this->BplusTree = bplusTree(this->tableName, thirdParam, this->rowCount, this->indexedColumnNumber);
        }
        else{
            // construct a hash
            // thirdParam -> number of buckets
            this->Hashing = hashing(this->tableName, stoi(thirdParam), this->rowCount, this->indexedColumnNumber);
            for (int i = 0; i < this->blockCount; i++)
            {
                cout << "ANNARAAAA" << endl;
            }
            for (int i = 0; i < this->blockCount; i++)
            {
                Cursor cursor(this->tableName, i); 
                for (int j = 0; j < this->rowCount; j++)
                {
                    vector <int> rr = cursor.getNext();
                    cout << "OKOKOKOKOKOKOKOK" << endl;
                    cout << rr[this->indexedColumnNumber] << endl;
                    this->Hashing.insertItem(rr[this->indexedColumnNumber],i,j);
                    cout << "RARARARAR";
                    cout << endl;
                    this->Hashing.displayHash();
                }
                
            }
            
            
        }
    }
}


int Table::insertRow(vector<int> values){
    logger.log("Table::insert");
    uint count = this->rowCount;

    //print headings
    // this->writeRow(this->columns, cout);
    // Cursor cursor(this->tableName, 0);
    // vector<int> row;
    // for (int rowCounter = 0; rowCounter < count; rowCounter++)
    // {
    //     row = cursor.getNext();
    //     this->writeRow(row, cout);
    // }
    // string FileName = "../data/temp/" + this->tableName + "_Page" + to_string(this->blockCount-1);
    
    cout << "row count of this table is "<< this->rowCount <<endl;
    this->rowCount++;
    for (auto d: this->rowsPerBlockCount){
        cout << d <<" " ;
    }
    cout << endl;
    this->rowsPerBlockCount[this->blockCount-1]++;
    cout << "row count of this table is "<< this->rowCount <<endl;
    for (auto d: this->rowsPerBlockCount){
        cout << d <<" " ;
    }
    cout << endl;
    Page lastPage =  bufferManager.getPage(this->tableName,this->blockCount-1);
    lastPage.insertPageRow(values);
    bufferManager.updatePage(this->tableName+"_Page"+to_string(this->blockCount-1),lastPage);




    // ofstream fout(FileName, ios::out);
    // this->writeRow(values,fout);
    // this->writeRow(values,cout);
    printRowCount(this->rowCount);   
}