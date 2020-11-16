#include "global.h"
#include <cstdio>
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
    this->thirdParam = "";
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
    // cout << "ARARARRA" << endl;
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
    this->columns.assign(this->columnCount, "");
    this->distinctValuesPerColumnCount.assign(this->columnCount, 0);
    getline(fin, line);
    stringstream s(line);
    // cout << "in blockify ZERO loop over ROWS HERE : " << this->maxRowsPerBlock << endl;
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
    // getline(fin, line);
    while (getline(fin, line))
    {
        stringstream s(line);
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (!getline(s, word, ','))
                return false;
            // cout << "OK" <<  word << endl;
            row[columnCounter] = stoi(word);
            // cout << "OKOK" << endl;
            rowsInPage[pageCounter][columnCounter] = row[columnCounter];
        }
        pageCounter++;
        this->updateStatistics(row);
        if (pageCounter == this->maxRowsPerBlock)
        {
            // cout << "in blockify ZERO ROWS HERE : " << pageCounter << endl;
            bufferManager.writePage(this->tableName, this->blockCount, rowsInPage, pageCounter);
            this->blockCount++;
            this->rowsPerBlockCount.emplace_back(pageCounter);
            pageCounter = 0;
        }
    }

    // cout << "in blockify second loop over" << endl;
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
    // cout << columnName;
    for (auto col : this->columns)
    {
        // cout << "PPPPPPPPPPPPPPPPPPPp";
        // cout << col;
        // cout << endl;
        if (col == columnName)
        {
            // cout << "OKOKOKOKOKOKO";
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
 * @brief This function writes rows onto the source File Name (directly)
 *
 * @param vector of rows
 */
void Table::writeRows(vector <vector<int>> rowsArr){
    string toWrite = "";
    int a= rowsArr.size();
    int b = rowsArr[0].size();
    for (int i=0;i<a;i++){
        for (int j=0;j<b;j++){
            toWrite += to_string(rowsArr[i][j]) + ",";
        }
        toWrite += "\n";
    }
    ofstream fout(this->sourceFileName, ios::app);
    fout << toWrite;
    fout.close();
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
    Cursor cursor(this->tableName, 0);
    vector<int> row;
    for (int rowCounter = 0; rowCounter < count; rowCounter++)
    {
        row = cursor.getNext();
        cout << "PRINTING ROW SIZE " << row.size() << "   " << this->columnCount << endl;
        this->writeRow(row, cout);
    }
    printRowCount(this->rowCount);
}

/**
 * @brief This function returns one row of the table using the cursor object. It
 * returns an empty row if all rows have been read.
 *
 * @param cursor 
 * @return vector<int> 
 */
void Table::getNextPage(Cursor *cursor)
{
    logger.log("Table::getNext");

    if (cursor->pageIndex < this->blockCount - 1)
    {
        cursor->nextPage(cursor->pageIndex + 1);
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
    if (!this->isPermanent())
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
void Table::unload()
{
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
 * @brief This function just checks indexed or not and prints it
 *
 * @return 0 
 */
int Table::checkIndex()
{
    cout << this->tableName << ";  :" << this->indexed << "  " << endl;
    return 0;
}


/**
 * @brief Function that returns the index of column indicated by indexName
 * 
 * @param  columnName
 * @param  indexingStrategy
 * @param  thirdParam: BUCKETS or FANOUT (in string)
 * @return int 
 */
int Table::indexTable(string columnName, IndexingStrategy indexingStrategy, string thirdParam)
{
    logger.log("Index::creatingIndex");
    if (indexingStrategy == NOTHING)
    {
        this->indexed = false;
        this->indexedColumn = "";
    }
    else
    {   
        this->indexed = true;
        this->indexedColumn = columnName;
        this->thirdParam = thirdParam;
        // cout << endl << endl << " I HAVE CHANGED INDEX VARISVLES " << this->indexed << "   " << this->indexedColumn << endl << endl;
        for (int i = 0; i < this->columnCount; i++)
        {
            if (this->columns[i] == this->indexedColumn)
            {
                this->indexedColumnNumber = i;
            }
        }
        if (indexingStrategy == BTREE)
        {
            // construct a btree
            // thirdParam -> fanout
            this->indexingStrategy = BTREE;
            this->BplusTree = bplusTree(this->tableName, thirdParam, this->rowCount, this->indexedColumnNumber);
            this->sortNoIndex(columnName,this->tableName,0);

            /*
            *
            *
            *   
            * this->sort_me();
            * 
            */
            // for (int i = 0; i < this->blockCount; i++)
            // {
            //     cout << "ANNARAAAA" << endl;
            // }

            bufferManager.unloadPages();

            for (int i = 0; i < this->blockCount; i++)
            {
                Page page = bufferManager.getPage(this->tableName,i);
                vector < vector <int> > rows = page.getRows();
                // cout << rows.size() << endl;
                int number = page.getRowCount();
                for (int j = 0; j < number; j++)
                {
                    pair <int,int> p = this->BplusTree.search(rows[j][this->indexedColumnNumber]);
                    if(p.first == -1){
                        // cout << "debug statement " << rows[j][this->indexedColumnNumber]  << endl;
                        // only insert if not already there
                        this->BplusTree.insert(rows[j][this->indexedColumnNumber],i,j);
                    }
                }
                
            }
            // this->BplusTree.removeKey(6);
            this->BplusTree.display(this->BplusTree.getRoot());
        }
        else
        {
            // construct a hash
            // thirdParam -> number of buckets
            this->indexingStrategy = HASH;
            this->Hashing = hashing(this->tableName, stoi(thirdParam), this->rowCount, this->indexedColumnNumber);
            for (int i = 0; i < this->blockCount; i++)
            {
                Page page = bufferManager.getPage(this->tableName,i);
                vector < vector <int> > rows = page.getRows();
                // cout << rows.size() << endl;
                int number = page.getRowCount();
                for (int j = 0; j < number; j++)
                {
                    this->Hashing.insertItem(rows[j][this->indexedColumnNumber],i,j);
                }
            }
            this->Hashing.displayHash();
            
        }
    }
}


/**
 * @brief Function that inserts at the last like a normal insert
 * 
 * @param  2D vector
 * @return pagePtr,rowPtr pair
 */
pair<int,int> Table::insertLast(vector<int> values)
{
    uint count = this->rowCount;

    cout << "row count of this table is " << this->rowCount << endl;
    this->rowCount++;
    int blkiter = 0;
    bool flag = 0;
    int wherePage = -1;
    int whereRow = -1;
    for (int itr = 0; itr < this->rowsPerBlockCount.size();itr++)
    {
        int d = this->rowsPerBlockCount[itr];
        cout << d << " ";
        if (d < this->maxRowsPerBlock)
        {
            cout << d << " ";
            flag = 1;
            wherePage = blkiter;
            whereRow = this->rowsPerBlockCount[blkiter];
            this->rowsPerBlockCount[blkiter]++;
            Page lastPage = bufferManager.getPage(this->tableName, blkiter);
            lastPage.insertPageRow(values);
            bufferManager.updatePage(this->tableName + "_Page" + to_string(blkiter), lastPage);
        }
        blkiter++;
    }
    cout << endl;
    // in case no block is vacant
    cout << "row count of this table is " << this->rowCount << endl;
    for (auto d : this->rowsPerBlockCount)
    {
        cout << d << " ";
    }
    if (flag == 0)
    {
        if(this->rowsPerBlockCount[this->blockCount - 1] >= this->maxRowsPerBlock){
            // new last page also full kya hi kar sakte hai
            vector < vector <int>> va;
            va.push_back(values);
            bufferManager.writePage(this->tableName,this->blockCount++,va,1);
            printRowCount(this->rowCount);
            return {this->blockCount - 1,0};
        }
    }
    else{
        return {wherePage,whereRow};
    }

}

/**
 * @brief Function that inserts a row of values into Table
 * 
 * @param  2D vector
 * @return int (yes or no) 
 */
int Table::insertRow(vector<int> values)
{
    logger.log("Table::insert");
    cout << "LOLOLOL\t " << this->indexingStrategy << endl;
    if(this->indexed){
        cout << "Indexed Table" << endl;
        if(this->indexingStrategy == BTREE){
            // do not insert at last, insert in the overflow page instead
            cout << "ME INSERT" << endl;
            pair <int,int> p = this->BplusTree.search(values[this->indexedColumnNumber]);
            cout << p.first << " " << p.second << endl;
            // pair<int,int> p = this->insertLast(values);
            Page page = bufferManager.getPage(this->tableName,-1);
            cout << page.getRowCount() << "\tROW COUNT" << endl;;
            if(page.getRowCount() == this->maxRowsPerBlock){
                // block full re-index
                
            }
            else{
                vector <vector <int> > rows;
                if(page.getRowCount() != 0){
                    rows = page.getRows();
                }
                pair <int,int> p = this->BplusTree.search(values[this->indexedColumnNumber]);
                rows.push_back(values);
                page.writeRows(rows, page.getRowCount() + 1);
                page.writePage();
            }
            return 0;
        }
        else
        {
            // hash
            pair<int,int> p = this->insertLast(values);
            cout << "HASH INDEX HUA HAI" << endl;
            cout << "wherePage:\t" << p.first << endl;
            cout << "whereRow:\t" << p.second << endl;
            this->Hashing.insertItem(values[this->indexedColumnNumber],p.first,p.second);
            this->Hashing.displayHash();
        }   
    }
    else
    {
        // not indexed, normal insert
        pair<int,int> p = this->insertLast(values);
    }
    

}

/**
 * @brief Sort function
 * 
 * @param  vector<int> &v1
 * @param  vector<int> &v2
 * @param  int k
 * @return bool
 */
bool sortcol(const vector<int> &v1, const vector<int> &v2, int k)
{
    return v1[k] < v2[k];
}

/**
 * @brief Function that returns the index of column indicated by indexName
 * 
 * @param  string columnName
 * @param  finName
 * @param  bool whether to insert or not 
 * @param int buffer size
 * @return int
 */
int Table::sortDesc(string columnName,string finName, bool toInsert , int buffersizeM ) /* toInsert =1  buffersizeM =10*/
{
    int blkiter = 0;
    int m = buffersizeM;
    // cout << "value of m is " << m << endl; 
    // cout << "got called \n\n";
    // cout << "row count of this table is " << this->rowCount << endl;
    // cout << this->rowsPerBlockCount[0] << endl;

    int indk = this->getColumnIndex(columnName);
    
    Table* phase1res = new Table("_Y"+this->tableName, this->columns);

    for (auto d : this->rowsPerBlockCount)
    {
        // cout << "page" << d << "\n";
        if (d > 0)
        {
            Page curPage = bufferManager.getPage(this->tableName, blkiter);
            vector<vector<int>> pageRows = curPage.getRows();
            pageRows = vector<vector<int>>(pageRows.begin(), pageRows.end()-pageRows.size()+d);
            int m = pageRows.size();
            int n = pageRows[0].size();
            // cout << "The Page rows before sorting are:\n";
            // for (int i = 0; i < m; i++)
            // {
            //     for (int j = 0; j < n; j++)
            //         cout << pageRows[i][j] << " ";
            //     cout << endl;
            // }

            // cout << "got page " << curPage.pageName << endl;
            
            sort(pageRows.begin(), pageRows.end(), [indk]( const vector<int> &v1, const vector<int> &v2){ return v1[indk] > v2[indk]; } );
            // cout << "The Page rows after sorting are:\n";
            // for (int i = 0; i < m; i++)
            // {
            //     for (int j = 0; j < n; j++)
            //         cout << pageRows[i][j] << " ";
            //     cout << endl;
            // }

            // TODO : problem is where to write the sorted page ..??
            phase1res->writeRows(pageRows);
            // bufferManager.writePage("Temp",blkiter,pageRows,d);

        }


        blkiter++;
    }

    phase1res->blockify();
    tableCatalogue.insertTable(phase1res);



    int chunkSize = 1;

    string writeTable = "_X"+this->tableName;
    string readTable = "_Y"+this->tableName;

    while (1)
    {
        Table* resultantTable = new Table(writeTable, this->columns); 
        int tillPage = 0;
        // covers one iteration over the entire table of merging the chunks of chunksize 
        while (1)
        {
            vector <Page> pageArr;
            vector <int> pageCount; 
            vector <int> pagePointer; 
            for (int i=0; i<(m-1) and i*chunkSize < this->blockCount-tillPage ;i++){
                // cout << "getting page " << chunkSize*i <<  " Block count:" << this->blockCount << " tillPage:"<< tillPage << " i:" << i <<  endl;
                // pageArr.insert(pageArr.begin()+i,bufferManager.getPage(this->tableName,tillPage+chunkSize*i));
                pageArr.insert(pageArr.begin()+i,bufferManager.getPage(readTable,tillPage+chunkSize*i));
                pageCount.insert(pageCount.begin()+i,0);
                pagePointer.insert(pagePointer.begin()+i,0);
                
            }       
            // cout << "got all pages\n";
            int rowsmerged =0;
            int sumrows = 0;
            for (int i=0; i< chunkSize*(m-1) and i < this->blockCount-tillPage ;i++){
                sumrows += this->rowsPerBlockCount[tillPage + i];
            }

            // merges the m-1 chunks of given chunksize 
            while (1){
                int minRow = -1;
                int minRowInd = -1;
                vector <int> minResRow;
                for  (int i=0;i<m-1 and i < this->blockCount-tillPage ;i++){
                    if (pageCount[i]>=chunkSize){
                        // cout << " I CONTINUED for " << i << endl;
                        continue;
                    }
                    else {
                    // cout << pageArr[i].pageName << " yaar "  <<endl; 
                    // cout << pageArr[i].pageName << " yaar " << pageArr[i].getRowCount() <<endl; 
                    vector <int> resrows = pageArr[i].getRow(pagePointer[i]) ;
                    // cout << "THIS IS ROW " << pagePointer[i] << ": " ;
                    // for (auto x : resrows ){
                    //     cout << x << " ";
                    // }
                    // cout << endl;
                    if (resrows[indk] > minRow ){
                        minRow = resrows[indk];
                        minRowInd = i;
                        minResRow = resrows;
                    }
                    }
                }
                // cout << " ==== PAGE NUM CALC are " << tillPage +pageCount[minRowInd]+ minRowInd*chunkSize << endl;
                // cout << "minimum row found at pagechunk " << minRowInd << " with value " << minRow << endl << endl;
                pagePointer[minRowInd]++;
                // TODO : write row takes time - directly write to page maybe if possible 
                resultantTable->writeRow(minResRow);

                //TODO Fix it - like which page to take next (chunkSize+minRowInd-1) not right 
                // Done 
                if (pagePointer[minRowInd] >= this->rowsPerBlockCount[tillPage +pageCount[minRowInd]+ minRowInd*chunkSize] ){
                    pageCount[minRowInd]++;
                    pagePointer[minRowInd]=0;
                    if (pageCount[minRowInd]<chunkSize ){
                        if (tillPage + chunkSize*minRowInd+pageCount[minRowInd]<this->blockCount){
                            // pageArr.insert(pageArr.begin()+minRowInd,bufferManager.getPage(readTable,tillPage + chunkSize*minRowInd+pageCount[minRowInd]));
                            pageArr[minRowInd]=bufferManager.getPage(readTable,tillPage + chunkSize*minRowInd+pageCount[minRowInd]);
                        }
                        else {
                            pageCount[minRowInd]=chunkSize;
                        }
                    }
                }

                rowsmerged ++ ;
                if (rowsmerged >= sumrows){
                    break;
                }  
            } 

            tillPage += chunkSize*(m-1);

            // long long int jpo = 0;
            // while (jpo<1e9){
            //     jpo++;
            // }

            if (tillPage >= this->blockCount)
                break;
        }

        // if(resultantTable->blockify())
        // tableCatalogue.insertTable(resultantTable);

        chunkSize = chunkSize*(m-1); 
        
        if (writeTable == "_X"+this->tableName){
            readTable = "_X"+this->tableName;
            // cout << "seg fault ?\n";

            if (chunkSize<this->blockCount || (1)){
                tableCatalogue.deleteTable("_Y"+this->tableName);
            }

            writeTable = "_Y"+this->tableName;
            // cout << "nope\n";

        }
        else {
            readTable = "_Y"+this->tableName;

            if (chunkSize<this->blockCount || (1)){
                tableCatalogue.deleteTable("_X"+this->tableName);
            }
            
            writeTable = "_X"+this->tableName;
        }


        if (chunkSize>=this->blockCount){
            // cout << "output in " << readTable << endl;
            
            //RENAMING OUTPUT
            resultantTable->tableName= finName ; 

            string ktemp = "../data/temp/"+readTable+".csv";
        	char oldname[ktemp.length()+1] ;
	        strcpy(oldname, ktemp.c_str());

            ktemp = "../data/temp/"+finName+".csv"; 
            char newname[ktemp.length()+1];
	        strcpy(newname, ktemp.c_str());
            
            // cout << "in blockify ?? " << endl;
            if(resultantTable->blockify()){
                // cout << "no blockify ?? " << endl;
                rename(oldname,newname);
                resultantTable->sourceFileName = "../data/"+finName+".csv";
                if (toInsert)
                    tableCatalogue.insertTable(resultantTable);
            }

            break;
        }

        else {
            if(resultantTable->blockify()){
            if (toInsert)
                tableCatalogue.insertTable(resultantTable);
            }
        }
    }
    
}

/**
 * @brief Function that sorts the table normally
 * 
 * @param  columnName
 * @param  finName
 * @param  toInsert
 * @param buffersizeM
 * @return int 
 */
int Table::sortNoIndex(string columnName,string finName, bool toInsert , int buffersizeM ) /* toInsert =1  buffersizeM =10*/
{
    int blkiter = 0;
    int m = buffersizeM;
    // cout << "value of m is " << m << endl; 
    // cout << "got called \n\n";
    // cout << "row count of this table is " << this->rowCount << endl;
    // cout << this->rowsPerBlockCount[0] << endl;

    int indk = this->getColumnIndex(columnName);
    
    Table* phase1res = new Table("_Y"+this->tableName, this->columns);

    for (auto d : this->rowsPerBlockCount)
    {
        // cout << "page" << d << "\n";
        if (d > 0)
        {
            Page curPage = bufferManager.getPage(this->tableName, blkiter);
            vector<vector<int>> pageRows = curPage.getRows();
            pageRows = vector<vector<int>>(pageRows.begin(), pageRows.end()-pageRows.size()+d);
            int m = pageRows.size();
            int n = pageRows[0].size();
            // cout << "The Page rows before sorting are:\n";
            // for (int i = 0; i < m; i++)
            // {
            //     for (int j = 0; j < n; j++)
            //         cout << pageRows[i][j] << " ";
            //     cout << endl;
            // }

            // cout << "got page " << curPage.pageName << endl;
            
            sort(pageRows.begin(), pageRows.end(), [indk]( const vector<int> &v1, const vector<int> &v2){ return v1[indk] < v2[indk]; } );
            // cout << "The Page rows after sorting are:\n";
            // for (int i = 0; i < m; i++)
            // {
            //     for (int j = 0; j < n; j++)
            //         cout << pageRows[i][j] << " ";
            //     cout << endl;
            // }

            // TODO : problem is where to write the sorted page ..??
            phase1res->writeRows(pageRows);
            // bufferManager.writePage("Temp",blkiter,pageRows,d);

        }


        blkiter++;
    }

    phase1res->blockify();
    tableCatalogue.insertTable(phase1res);



    int chunkSize = 1;

    string writeTable = "_X"+this->tableName;
    string readTable = "_Y"+this->tableName;

    while (1)
    {
        Table* resultantTable = new Table(writeTable, this->columns); 
        int tillPage = 0;
        // covers one iteration over the entire table of merging the chunks of chunksize 
        while (1)
        {
            vector <Page> pageArr;
            vector <int> pageCount; 
            vector <int> pagePointer; 
            for (int i=0; i<(m-1) and i*chunkSize < this->blockCount-tillPage ;i++){
                // cout << "getting page " << chunkSize*i <<  " Block count:" << this->blockCount << " tillPage:"<< tillPage << " i:" << i <<  endl;
                // pageArr.insert(pageArr.begin()+i,bufferManager.getPage(this->tableName,tillPage+chunkSize*i));
                pageArr.insert(pageArr.begin()+i,bufferManager.getPage(readTable,tillPage+chunkSize*i));
                pageCount.insert(pageCount.begin()+i,0);
                pagePointer.insert(pagePointer.begin()+i,0);
                
            }       
            // cout << "got all pages\n";
            int rowsmerged =0;
            int sumrows = 0;
            for (int i=0; i< chunkSize*(m-1) and i < this->blockCount-tillPage ;i++){
                sumrows += this->rowsPerBlockCount[tillPage + i];
            }

            // merges the m-1 chunks of given chunksize 
            while (1){
                int minRow = INT_MAX;
                int minRowInd = -1;
                vector <int> minResRow;
                for  (int i=0;i<m-1 and i < this->blockCount-tillPage ;i++){
                    if (pageCount[i]>=chunkSize){
                        // cout << " I CONTINUED for " << i << endl;
                        continue;
                    }
                    else {
                    // cout << pageArr[i].pageName << " yaar "  <<endl; 
                    // cout << pageArr[i].pageName << " yaar " << pageArr[i].getRowCount() <<endl; 
                    vector <int> resrows = pageArr[i].getRow(pagePointer[i]) ;
                    // cout << "THIS IS ROW " << pagePointer[i] << ": " ;
                    // for (auto x : resrows ){
                    //     cout << x << " ";
                    // }
                    // cout << endl;
                    if (resrows[indk] < minRow ){
                        minRow = resrows[indk];
                        minRowInd = i;
                        minResRow = resrows;
                    }
                    }
                }
                // cout << " ==== PAGE NUM CALC are " << tillPage +pageCount[minRowInd]+ minRowInd*chunkSize << endl;
                // cout << "minimum row found at pagechunk " << minRowInd << " with value " << minRow << endl << endl;
                pagePointer[minRowInd]++;
                // TODO : write row takes time - directly write to page maybe if possible 
                resultantTable->writeRow(minResRow);

                //TODO Fix it - like which page to take next (chunkSize+minRowInd-1) not right 
                // Done 
                if (pagePointer[minRowInd] >= this->rowsPerBlockCount[tillPage +pageCount[minRowInd]+ minRowInd*chunkSize] ){
                    pageCount[minRowInd]++;
                    pagePointer[minRowInd]=0;
                    if (pageCount[minRowInd]<chunkSize ){
                        if (tillPage + chunkSize*minRowInd+pageCount[minRowInd]<this->blockCount){
                            // pageArr.insert(pageArr.begin()+minRowInd,bufferManager.getPage(readTable,tillPage + chunkSize*minRowInd+pageCount[minRowInd]));
                            pageArr[minRowInd]=bufferManager.getPage(readTable,tillPage + chunkSize*minRowInd+pageCount[minRowInd]);
                        }
                        else {
                            pageCount[minRowInd]=chunkSize;
                        }
                    }
                }

                rowsmerged ++ ;
                if (rowsmerged >= sumrows){
                    break;
                }  
            } 

            tillPage += chunkSize*(m-1);

            // long long int jpo = 0;
            // while (jpo<1e9){
            //     jpo++;
            // }

            if (tillPage >= this->blockCount)
                break;
        }

        // if(resultantTable->blockify())
        // tableCatalogue.insertTable(resultantTable);

        chunkSize = chunkSize*(m-1); 
        
        if (writeTable == "_X"+this->tableName){
            readTable = "_X"+this->tableName;
            // cout << "seg fault ?\n";

            if (chunkSize<this->blockCount || (1)){
                tableCatalogue.deleteTable("_Y"+this->tableName);
            }

            writeTable = "_Y"+this->tableName;
            // cout << "nope\n";

        }
        else {
            readTable = "_Y"+this->tableName;

            if (chunkSize<this->blockCount || (1)){
                tableCatalogue.deleteTable("_X"+this->tableName);
            }
            
            writeTable = "_X"+this->tableName;
        }


        if (chunkSize>=this->blockCount){
            // cout << "output in " << readTable << endl;
            
            //RENAMING OUTPUT
            resultantTable->tableName= finName ; 

            string ktemp = "../data/temp/"+readTable+".csv";
        	char oldname[ktemp.length()+1] ;
	        strcpy(oldname, ktemp.c_str());

            ktemp = "../data/temp/"+finName+".csv"; 
            char newname[ktemp.length()+1];
	        strcpy(newname, ktemp.c_str());
            
            // cout << "in blockify ?? " << endl;
            if(resultantTable->blockify()){
                // cout << "no blockify ?? " << endl;
                rename(oldname,newname);
                resultantTable->sourceFileName = "../data/"+finName+".csv";
                if (toInsert)
                    tableCatalogue.insertTable(resultantTable);
            }

            break;
        }

        else {
            if(resultantTable->blockify()){
            if (toInsert)
                tableCatalogue.insertTable(resultantTable);
            }
        }
    }
    
}

/**
 * @brief Function that add the index of column indicated by indexName
 * 
 * @param  columnName
 * @return int 
 */
int Table::addCol(string columnName){
    cout << "begin operation alter"<< endl;
    cout << columnName << " is to be added "<< endl;
    // for (auto d:this->columns){
    //     cout << d << " ";
    // }

    vector <string> newCol = this->columns;
    newCol.emplace_back(columnName);
    Table * resultantTable = new Table("A_temp"+this->tableName, newCol); 
    
    // cout << endl;
    // cout << "DIS VAL PUSH"<< endl;
    int blkiter=0;
    bool flag = 0;    
    // cout << "meta data altered"<< endl;
    for (auto d : this->rowsPerBlockCount)
    {
        // cout << d << " PRINT D VALUE " << endl;

        flag = 1;
        Page lastPage = bufferManager.getPage(this->tableName, blkiter);
        vector<vector<int>> rowset1 = lastPage.getRows();
        vector<vector<int>> rowset;

        for (int i =0; i < d;i++){
            rowset.push_back(rowset1[i]);
            rowset[i].push_back(0);
            // for (auto lol : rowset[i]){
            //     cout << lol << " ";
            // }
            // cout << "updates << " << endl;
        }

        // for (auto rt : rowset){
        //     for (auto lol : rt){
        //         cout << lol << " ";
        //     }
        //     cout << endl;
        // }

        resultantTable->writeRows(rowset);
        
        // lastPage.writeToPage(rowset);
        // bufferManager.updatePage(this->tableName + "_Page" + to_string(blkiter), lastPage);

        blkiter++;
    }
    
    // resultantTable->tableName=this->tableName;
    resultantTable->blockify();

    string nameOG = this->tableName;
    tableCatalogue.deleteTable(this->tableName);

    // resultantTable->tableName= this->tableName ; 
    // /*
    blkiter=0;
    for (auto d : resultantTable->rowsPerBlockCount){

        // Page newPage = bufferManager.getPage(resultantTable->tableName,blkiter);
        // cout << "segway\n";
        // cout << "PAGE ROWS : "  << newPage.getRowCount()  << endl;
        // cout << "PAGE ROWS : "  << d  << endl;


        string ktemp = "../data/temp/"+resultantTable->tableName+"_Page"+to_string(blkiter);
        char oldname[ktemp.length()+1] ;
        strcpy(oldname, ktemp.c_str());

        ktemp = "../data/temp/"+nameOG+"_Page"+to_string(blkiter); 
        char newname[ktemp.length()+1];
        strcpy(newname, ktemp.c_str());

        rename(oldname,newname);    

        bufferManager.unloadPage("../data/temp/"+nameOG+"_Page"+to_string(blkiter));

        // bufferManager.updatePage("../data/temp/"+this->tableName+"_Page"+to_string(blkiter),newPage);


        blkiter++;  
    }


    resultantTable->tableName=nameOG;
    tableCatalogue.insertTable(resultantTable);
    // this->columns.emplace_back(columnName);
    cout << "PUSHED"<< endl;
    // Page lkl = bufferManager.getPage(nameOG,18);
    // vector <vector<int>> polo = lkl.getRows();
    // for (auto lk : polo){
    //     for (auto pol : lk){
    //         cout << pol << " ";
    //     }
    //     cout << endl;
    // } 
    // cout << "PRINTED LAST PADE \n";
    // this->columnCount++;
    // this->distinctValuesPerColumnCount.push_back(1);
    // this->maxRowsPerBlock
    // */

    return 0;

}


/**
 * @brief Function that checks whether same or not
 * 
 * @param  2D vector of rows
 * @param  rowsCount
 * @param  values
 * @return pair of pagePtr, rowPtr
 */
pair<int,int> Table::checkSame(vector< vector<int>> rows, int rowsCount, vector<int> values){
    int found = 0;
    int where = -1;
    int gggg = 1;
    for (int i = 0;!found && i < rowsCount; i++)
    {
        found = 1;
        for (int j = 0; j < this->columnCount; j++)
        {
            if(rows[i][j] != values[j]){
                found = 0;
            }
        }
        if(found){
            where = i;
        }
    }
    return {found,where};
}


int Table::deleteRow(vector <int> values){
    int found = 0;
    if(this->indexed){
        // it is indexed
        if(this->indexingStrategy == BTREE){
            // BTREE
            pair<int,int> p = this->BplusTree.search(values[this->indexedColumnNumber]);
            if(p.first == -1){
                // not in Table, check in Overflow block
                Page page = bufferManager.getPage(this->tableName,-1);
                vector < vector< int> > rows = page.getRows();
                int found = 1;
                for (int i = 0; !found && i < page.getRowCount(); i++)
                {
                    found = 1;
                    for (int j = 0; j < this->columnCount; j++)
                    {
                        if(values[j] != rows[i][j]){
                            found = 0;
                        }
                    }
                    if(found){
                        // Page page = bufferManager.getPage(this->tableName,pageNum);
                        // vector < vector<int> > rows = page.getRows();
                        rows.erase(rows.begin() + i);
                        page.writeRows(rows,page.getRowCount() - 1);
                        page.writePage();
                        bufferManager.updatePage(this->tableName + "_Page" + to_string(-1),page);                       
                    }                    
                }

            }
            else{
                // in main block 
                int curval = values[indexedColumnNumber];
                int ind = p.first; // get this page
                int val = curval; // this is cur val of indexed column
                int startRow = 0; // start from where row is queried
                int found = 0;
                while (!found && curval == val && ind < this->blockCount)
                {
                    /* code */
                    found = 1;
                    Page page = bufferManager.getPage(this->tableName,ind);
                    vector < vector <int >> rows = page.getRows();
                    pair<int,int> p = checkSame(rows,this->rowsPerBlockCount[ind],values);   
                    if(p.first = 1){
                        // found here
                        found = 1;
                        int where = p.second;
                        rows.erase(rows.begin() + where);
                        this->rowsPerBlockCount[ind]--;
                        page.writeRows(rows,page.getRowCount() - 1);
                        page.writePage();
                        bufferManager.updatePage(this->tableName + "_Page" + to_string(ind),page);
                        // now update all trailing elements
                        for (int i = where; i < rowsPerBlockCount[ind]; i++)
                        {
                            this->BplusTree.searchAndUpdate(rows[i][this->indexedColumnNumber],ind,i);
                        }
                    }
                    else{
                        val = rows[this->rowsPerBlockCount[ind]][this->indexedColumnNumber];
                        ind++;
                    }
                }
                if(!found){
                    cout << "NOT FOUND SAARRY :(((" << endl;
                }
            }
        }
        else{
            // HASH
            vector< pair<int,int> > p = this->Hashing.searchElement(values[this->indexedColumnNumber]);
            int found = 0;
            for (int i = 0; !found && i < p.size(); i++)
            {
                found = 1;
                int pageNum = p[i].first;
                int rowNum = p[i].second;
                cout << "PageNum:\t" << pageNum << endl;
                cout << "RowNum:\t" << rowNum << endl;
                Page page = bufferManager.getPage(this->tableName,pageNum);
                vector < vector<int> > rows = page.getRows();
                
                for (int i = 0; !found && i < this->columnCount; i++)
                {
                    if(rows[rowNum][i] != values[i]){
                        // this is the row to be deleted
                        found = 0;
                    }
                }
                if(found){
                    // delete this
                    cout << "DELETING" << endl;
                    Page page = bufferManager.getPage(this->tableName,pageNum);
                    vector < vector<int> > rows = page.getRows();
                    rows.erase(rows.begin() + rowNum);
                    page.writeRows(rows,page.getRowCount() - 1);
                    page.writePage();
                    bufferManager.updatePage(this->tableName + "_Page" + to_string(pageNum),page);
                }
            }
        }
    }
    else{
        // not indexed
        // just delete from the page where it belonged to
        for (int i = 0; i < this->blockCount && !found; i++)
        {
            Page page = bufferManager.getPage(this->tableName,i);
            // cout << i << "`LALAL" << endl;
            vector < vector<int> > rows = page.getRows();
            cout << this->rowsPerBlockCount[i] << endl;
            for (int j = 0; j < this->rowsPerBlockCount[i] && !found; j++)
            {
                /* code */
                found = 1;
                for (int k = 0; k < this->columnCount; k++)
                {
                    if(rows[j][k] != values[k]){
                        found = 0;
                    }
                }
                if(found){
                    this->rowsPerBlockCount[i]--;
                    rows.erase(rows.begin() + j);
                    page.writeRows(rows,this->rowsPerBlockCount[i] - 1);
                    page.writePage();
                    bufferManager.updatePage(this->tableName + "_Page" + to_string(i),page);
                    cout << "Deleted from Page:\t" <<  i << endl;
                    cout << "Deleted from Row:\t" << j << endl;
                }
            }
        }
    }
    return found;
}

/**
 * @brief Function that returns the index of column indicated by indexName
 * 
 * @param  columnName
 * @return int 
 */
int Table::deleteCol(string columnName){
    // cout << "begin operation alter"<< endl;
    // cout << columnName << " is to be deleted "<< endl;
    // for (auto d:this->columns){
    //     cout << d << " ";
    // }
    int colind = this->getColumnIndex(columnName);

    vector <string> newCol = this->columns;
    newCol.erase(newCol.begin()+colind);
    Table * resultantTable = new Table("A_temp"+this->tableName, newCol); 
    
    // cout << endl;
    // cout << "DIS VAL PUSH"<< endl;
    int blkiter=0;
    bool flag = 0;    
    // cout << "meta data altered"<< endl;
    for (auto d : this->rowsPerBlockCount)
    {
        // cout << d << " PRINT D VALUE " << endl;

        flag = 1;
        Page lastPage = bufferManager.getPage(this->tableName, blkiter);
        vector<vector<int>> rowset1 = lastPage.getRows();
        vector<vector<int>> rowset;

        for (int i =0; i < d;i++){
            rowset.push_back(rowset1[i]);
            // rowset[i].push_back(0);
            rowset[i].erase(rowset[i].begin()+colind);
            // for (auto lol : rowset[i]){
            //     cout << lol << " ";
            // }
            // cout << "updates << " << endl;
        }

        // for (auto rt : rowset){
        //     for (auto lol : rt){
        //         cout << lol << " ";
        //     }
        //     cout << endl;
        // }

        resultantTable->writeRows(rowset);
        
        // lastPage.writeToPage(rowset);
        // bufferManager.updatePage(this->tableName + "_Page" + to_string(blkiter), lastPage);

        blkiter++;
    }
    

    resultantTable->blockify();


    string nameOG = this->tableName;
    tableCatalogue.deleteTable(this->tableName);

    // resultantTable->tableName= this->tableName ; 
    // /*
    blkiter=0;
    for (auto d : resultantTable->rowsPerBlockCount){

        // Page newPage = bufferManager.getPage(resultantTable->tableName,blkiter);


        string ktemp = "../data/temp/"+resultantTable->tableName+"_Page"+to_string(blkiter);
        char oldname[ktemp.length()+1] ;
        strcpy(oldname, ktemp.c_str());

        ktemp = "../data/temp/"+nameOG+"_Page"+to_string(blkiter); 
        char newname[ktemp.length()+1];
        strcpy(newname, ktemp.c_str());

        rename(oldname,newname);    

        bufferManager.unloadPage("../data/temp/"+nameOG+"_Page"+to_string(blkiter));

        // bufferManager.updatePage("../data/temp/"+this->tableName+"_Page"+to_string(blkiter),newPage);


        blkiter++;  
    }



    resultantTable->tableName=nameOG;
    tableCatalogue.insertTable(resultantTable);
    // this->columns.emplace_back(columnName);
    cout << "PUSHED"<< endl;
    // Page lkl = bufferManager.getPage(nameOG,18);
    // vector <vector<int>> polo = lkl.getRows();
    // for (auto lk : polo){
    //     for (auto pol : lk){
    //         cout << pol << " ";
    //     }
    //     cout << endl;
    // } 
    cout << "PRINTED LAST PADE \n";


    // this->columns.emplace_back(columnName);
    // this->columns.erase(this->columns.begin()+colind);
    // cout << "PUSHED"<< endl;
    // this->columnCount--;
    // this->distinctValuesPerColumnCount.erase(this->distinctValuesPerColumnCount.begin()+colind);
    // */

    return 0;

}
