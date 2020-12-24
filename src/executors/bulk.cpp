#include "global.h"
/**
 * @brief 
 * SYNTAX: BULK_INSERT relation_name
 */
bool syntacticParseBULK_INSERT()
{
    logger.log("syntacticParseBULK_INSERT");
    if (tokenizedQuery.size() != 4 || tokenizedQuery[2] != "INTO")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = BULK_INSERT;
    parsedQuery.bulkFromRelationName = tokenizedQuery[1];
    parsedQuery.bulkInsertRelationName = tokenizedQuery[3];
    return true;
}

bool semanticParseBULK_INSERT()
{
    logger.log("semanticParseBULK_INSERT");
    if (!tableCatalogue.isTable(parsedQuery.bulkInsertRelationName))
    {
        cout << "SEMANTIC ERROR: Relation does not exists" << endl;
        return false;
    }

    if (!isFileExists(parsedQuery.bulkFromRelationName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeBULK_INSERT()
{
    logger.log("executeLOAD");
    Table *table = tableCatalogue.getTable(parsedQuery.bulkInsertRelationName);
    bool indexed = table->indexed;
    IndexingStrategy indexingStrategy = table->indexingStrategy;
    string thirdParam   = table->thirdParam;
    string indexedColumn = table->indexedColumn;

    fstream newfile;
    newfile.open("../data/"+parsedQuery.bulkFromRelationName+".csv",ios::in);

    string word;

    bool lpflag =1;


    int num;
    string input;
    vector<int> row(table->columnCount, 0);
    vector<vector<int>> lastPageRows (table->maxRowsPerBlock, row);
    int pageCounter=0;



    //last block got space
    if (table->rowsPerBlockCount[table->blockCount - 1] < table->maxRowsPerBlock)
    {
        lpflag = 0;
        num = table->maxRowsPerBlock - table->rowsPerBlockCount[table->blockCount - 1];
        
        Page lastpage = bufferManager.getPage(table->tableName,table->blockCount-1);
        lastPageRows = lastpage.getRows();  
        int lpcount = table->rowsPerBlockCount[table->blockCount - 1];

        vector<int> row(table->columnCount, 0);
        pageCounter = table->rowsPerBlockCount[table->blockCount - 1];

    }

    //getting columns 
    getline(newfile,input);

    while  (getline(newfile,input))
    {
        stringstream s(input);
        for (int columnCounter = 0; columnCounter < table->columnCount; columnCounter++)
        {
            
            if (!getline(s, word, ','))
                return ;
            row[columnCounter] = stoi(word);
            lastPageRows[pageCounter][columnCounter] = row[columnCounter];

            
        }
        pageCounter++;


        //last page full
        if (pageCounter == table->maxRowsPerBlock && !lpflag  ){

            lpflag =1;        

            // bufferManager.updatePage();
            bufferManager.writePage(table->tableName, table->blockCount-1, lastPageRows, pageCounter);

            table->rowsPerBlockCount[table->blockCount-1] = table->maxRowsPerBlock;
            pageCounter = 0;
            // lastPageRows.clear()
                        
        }



        //writing to new page 
        else if (pageCounter == table->maxRowsPerBlock && lpflag  ){
            bufferManager.writePage(table->tableName, table->blockCount, lastPageRows , pageCounter);
            table->blockCount++;
            table->rowsPerBlockCount.emplace_back(pageCounter);
            pageCounter = 0;                            
        }

    }
    if (pageCounter)
    {
        bufferManager.writePage(table->tableName, table->blockCount, lastPageRows , pageCounter);
        table->blockCount++;
        table->rowsPerBlockCount.emplace_back(pageCounter);
        pageCounter = 0;
    }
    if(indexed){
        Table * tabl = tableCatalogue.getTable(parsedQuery.bulkInsertRelationName);
        if(tabl){
            tabl->indexTable(indexedColumn,indexingStrategy,thirdParam);
        }
    }

}
    






/*




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
    cout << "in blockify ZERO loop over ROWS HERE : " << this->maxRowsPerBlock << endl;
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
            cout << "in blockify ZERO ROWS HERE : " << pageCounter << endl;
            bufferManager.writePage(this->tableName, this->blockCount, rowsInPage, pageCounter);
            this->blockCount++;
            this->rowsPerBlockCount.emplace_back(pageCounter);
            pageCounter = 0;
        }
    }

    cout << "in blockify second loop over" << endl;
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










    // if (table->load())
    // {
    //     tableCatalogue.insertTable(table);
    //     cout << "Loaded Table. Column Count: " << table->columnCount << " Row Count: " << table->rowCount << endl;
    // }
    return;
}
*/