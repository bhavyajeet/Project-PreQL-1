#include "global.h"

/**
 * @brief Construct a new Matrix:: Matrix object
 *
 */
Matrix::Matrix()
{
    logger.log("Matrix::Matrix1");
}

/**
 * @brief Construct a new Matrix:: Matrix object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param MatrixName 
 */
Matrix::Matrix(string MatrixName)
{
    logger.log("Matrix::Matrix2");
    this->sourceFileName = "../data/" + MatrixName + ".csv";
    this->MatrixName = MatrixName;
}

/**
 * @brief Construct a new Matrix:: Matrix object used when an assignment command
 * is encountered. To create the Matrix object both the Matrix name and the
 * columns the Matrix holds should be specified.
 *
 * @param MatrixName 
 * @param columns 
 */
Matrix::Matrix(string MatrixName, vector<string> columns)
{
    logger.log("Matrix::Matrix3");
    this->sourceFileName = "../data/temp/" + MatrixName + ".csv";
    this->MatrixName = MatrixName;
    this->columns = columns;
    this->columnCount = columns.size();
    this->maxRowsPerBlock = (uint)(MAX_ROWS_MATRIX);
    this->maxColumnsPerBlock = (uint)(MAX_COLS_MATRIX);
    this->writeRow<string>(columns);
}

/**
 * @brief The load function is used when the LOAD command is encountered. It
 * reads data from the source file, splits it into blocks and updates Matrix
 * statistics.
 *
 * @return true if the Matrix has been successfully loaded 
 * @return false if an error occurred 
 */
bool Matrix::load()
{
    logger.log("Matrix::load");
    logger.log(this->sourceFileName);
    fstream fin(this->sourceFileName, ios::in);
    string line;
    if (getline(fin, line))
    {
        cout << "getting there";
        fin.close();
       if (this->extractColumnNames(line)){
        cout << "getting there PART 2";
        if (this->blockify())
            return true;
       }
    }
    fin.close();
    logger.log(line);
    cout << line;
    cout << "lol";
    logger.log("YTE KYUN HUA");
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
bool Matrix::extractColumnNames(string firstLine)
{
    logger.log("Matrix::extractColumnNames");
    unordered_set<string> columnNames;
    string word;
    stringstream s(firstLine);
    while (getline(s, word, ','))
    {
        word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
        cout << word << " HI RA " << columnNames.count(word) << endl;
        columnNames.insert(word);
        this->columns.emplace_back(word);
    }
    this->columnCount = this->columns.size();
    this->maxRowsPerBlock = (uint)(MAX_ROWS_MATRIX);
    this->maxColumnsPerBlock = (uint)(MAX_COLS_MATRIX);
    return true;
}

/**
 * @brief This function splits all the rows and stores them in multiple files of
 * one block size. 
 *
 * @return true if successfully blockified
 * @return false otherwise
 */
bool Matrix::blockify()
{   
    logger.log("Matrix::blockify");
    logger.log(to_string(this->columnCount));
    // cout << "col ocunt " << (to_string(this->columnCount))<< endl;
    string line, word;
    vector<int> row(min(this->maxColumnsPerBlock,this->columnCount), -1);
    vector<vector<int>> rowsInPage(this->maxRowsPerBlock, row);
    int pageCounter = 0;
    int currentColumn = 0;
    unordered_set<int> dummy;
    dummy.clear();
    this->distinctValuesInColumns.assign(this->columnCount, dummy);
    this->distinctValuesPerColumnCount.assign(this->columnCount, 0);
    logger.log(to_string(currentColumn));
    cout << "curr col " << (to_string(currentColumn)) << endl;
    logger.log(to_string(this->columnCount / this->maxColumnsPerBlock));
    int limit = this->columnCount / this->maxColumnsPerBlock;
    cout << "calc col " << (to_string(limit)) << endl;
    if(this->columnCount % this->maxColumnsPerBlock){
        limit++;
        cout << "gotta -1" << endl;
    }
    while(currentColumn < limit){
        logger.log(to_string(currentColumn));
        ifstream fin(this->sourceFileName, ios::in);
        int rowcounter =0 ;
        while (rowcounter<limit * this->maxRowsPerBlock)
        {
            rowcounter++;
            if (!getline(fin, line)){
                cout << "wanted to row ";
                cout << line;
            stringstream s(line);
            cout << rowcounter <<endl;
            for (int columnCounter = 0; columnCounter < limit * this->maxColumnsPerBlock ; columnCounter++)
            {
                // cout << "debug 1 "  << "at col" << columnCounter<< endl;
                if (!getline(s, word, ',') && columnCounter/this->maxColumnsPerBlock == currentColumn)
                {
                    // return false;
                    cout << word << "added -1 ig \n";
                    row[columnCounter % this->maxColumnsPerBlock] = -1;
                    rowsInPage[pageCounter % this->maxRowsPerBlock][columnCounter % this->maxColumnsPerBlock] = row[columnCounter % this->maxColumnsPerBlock];
                }
                else {
                    word = "-1";
                if(columnCounter/this->maxColumnsPerBlock == currentColumn){
                    row[columnCounter % this->maxColumnsPerBlock] = stoi(word);
                    rowsInPage[pageCounter % this->maxRowsPerBlock][columnCounter % this->maxColumnsPerBlock] = row[columnCounter % this->maxColumnsPerBlock];
                    cout << word << "notee\n";;
                }
                }
            }
            }
            else {
            cout << line;
            stringstream s(line);
            cout << rowcounter <<endl;
            for (int columnCounter = 0; columnCounter < limit * this->maxColumnsPerBlock ; columnCounter++)
            {
                // cout << "debug 1 "  << "at col" << columnCounter<< endl;
                if (!getline(s, word, ',') && columnCounter/this->maxColumnsPerBlock == currentColumn)
                {
                    // return false;
                    cout << word << "added -1 ig \n";
                    row[columnCounter % this->maxColumnsPerBlock] = -1;
                    rowsInPage[pageCounter % this->maxRowsPerBlock][columnCounter % this->maxColumnsPerBlock] = row[columnCounter % this->maxColumnsPerBlock];
                }
                else {
                if(columnCounter/this->maxColumnsPerBlock == currentColumn){
                    row[columnCounter % this->maxColumnsPerBlock] = stoi(word);
                    rowsInPage[pageCounter % this->maxRowsPerBlock][columnCounter % this->maxColumnsPerBlock] = row[columnCounter % this->maxColumnsPerBlock];
                    cout << word << "notee\n";;
                }
                }
            }
            }
            pageCounter++;
            logger.log("OK NA");
            this->updateStatistics(row);
            if (pageCounter % this->maxRowsPerBlock == 0)
            {
                logger.log("RARA");
                bufferManager.writePage(this->MatrixName, this->blockCount, rowsInPage, this->maxRowsPerBlock);
                this->blockCount++;
                this->rowsPerBlockCount.emplace_back(pageCounter);
            }
            logger.log("OHOH");
        }
        if (pageCounter % this->maxRowsPerBlock)
        {
            bufferManager.writePage(this->MatrixName, this->blockCount, rowsInPage, this->maxRowsPerBlock);
            this->blockCount++;
            this->rowsPerBlockCount.emplace_back(pageCounter);
            pageCounter = 0;
        }
        currentColumn++;        
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
void Matrix::updateStatistics(vector<int> row)
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
 * @brief Checks if the given column is present in this Matrix.
 *
 * @param columnName 
 * @return true 
 * @return false 
 */
bool Matrix::isColumn(string columnName)
{
    logger.log("Matrix::isColumn");
    for (auto col : this->columns)
    {
        if (col == columnName)
        {
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
void Matrix::renameColumn(string fromColumnName, string toColumnName)
{
    logger.log("Matrix::renameColumn");
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
 * @brief Function prints the first few rows of the Matrix. If the Matrix contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Matrix::print()
{
    logger.log("Matrix::print");
    uint count = min((long long)PRINT_COUNT, this->rowCount);

    //print headings
    this->writeRow(this->columns, cout);

    Cursor cursor(this->MatrixName, 0);
    vector<int> row;
    for (int rowCounter = 0; rowCounter < count; rowCounter++)
    {
        row = cursor.getNext();
        this->writeRow(row, cout);
    }
    printRowCount(this->rowCount);
}



/**
 * @brief This function returns one row of the Matrix using the cursor object. It
 * returns an empty row is all rows have been read.
 *
 * @param cursor 
 * @return vector<int> 
 */
void Matrix::getNextPage(Cursor *cursor)
{
    logger.log("Matrix::getNext");

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
void Matrix::makePermanent()
{
    logger.log("Matrix::makePermanent");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->MatrixName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    Cursor cursor(this->MatrixName, 0);
    vector<int> row;
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        row = cursor.getNext();
        this->writeRow(row, fout);
    }
    fout.close();
}

/**
 * @brief Function to check if Matrix is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Matrix::isPermanent()
{
    logger.log("Matrix::isPermanent");
    if (this->sourceFileName == "../data/" + this->MatrixName + ".csv")
    return true;
    return false;
}

/**
 * @brief The unload function removes the Matrix from the database by deleting
 * all temporary files created as part of this Matrix
 *
 */
void Matrix::unload(){
    logger.log("Matrix::~unload");
    for (int pageCounter = 0; pageCounter < this->blockCount; pageCounter++)
        bufferManager.deleteFile(this->MatrixName, pageCounter);
    if (!isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
}

/**
 * @brief Function that returns a cursor that reads rows from this Matrix
 * 
 * @return Cursor 
 */
Cursor Matrix::getCursor()
{
    logger.log("Matrix::getCursor");
    Cursor cursor(this->MatrixName, 0);
    return cursor;
}

/**
 * @brief Function that returns the index of column indicated by columnName
 * 
 * @param columnName 
 * @return int 
 */
int Matrix::getColumnIndex(string columnName)
{
    logger.log("Matrix::getColumnIndex");
    for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
    {
        if (this->columns[columnCounter] == columnName)
            return columnCounter;
    }
}


/**
 * @brief Function that transposes the matrix
 * 
 * @param columnName 
 * @return int 
 */
void Matrix::transpose(){
    logger.log("Matrix::transpose");
    logger.log("MAINE TRANSPOSE LIYA HAI");
    logger.log("OK NA");
    logger.log(this->MatrixName);
    logger.log(to_string(this->blockCount));
    for (int variable = 0; variable < this->blockCount; variable++)
    {
        // swap internally
        Page pagei = bufferManager.getPage(this->MatrixName,variable);
        pagei.swapElements();
    }
    int n;
    for (n = 0; n*n < this->blockCount; n++)
    {
        /* code */
    }    
    for (int variable = 0; variable < this->blockCount/2;variable++){
        // swap Page i and Page N*N - i
        if((variable % (n + 1))){ // not a diagonal number
            
            Page page1 = bufferManager.getPage(this->MatrixName,variable); // read i
            Page page2 = bufferManager.getPage(this->MatrixName,this->blockCount - 1 - variable); // read N*N - i
            vector<int> row(min(this->maxColumnsPerBlock,this->columnCount), -1);
            vector<vector<int>> matrix1(this->maxRowsPerBlock, row);
            vector<vector<int>> matrix2(this->maxRowsPerBlock, row);
            for (int i = 0; i < MAX_ROWS_MATRIX; i++)
            {
                /* code */
                // iterate over rows to get each row
                matrix1[i] = page1.getRow(i);
                matrix2[i] = page2.getRow(i);
            }
            page1.writeToPage(matrix2);
            page2.writeToPage(matrix1);
        }
    }
}