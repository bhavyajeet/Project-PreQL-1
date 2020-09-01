#include "global.h"
/**
 * @brief Construct a new Page object. Never used as part of the code
 *
 */
Page::Page()
{
    this->pageName = "";
    this->tableName = "";
    this->MatrixName = "";
    this->pageIndex = -1;
    this->rowCount = 0;
    this->columnCount = 0;
    this->rows.clear();
}

/**
 * @brief Construct a new Page:: Page object given the table name and page
 * index. When tables are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<tablename>_Page<pageindex>". For example, If the Page being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_Page2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param tableName 
 * @param pageIndex 
 */
Page::Page(string tableName, int pageIndex)
{
    logger.log("Page::Pageee");
    this->pageIndex = pageIndex;
    this->pageName = "../data/temp/" + tableName + "_Page" + to_string(pageIndex);
    logger.log(this->pageName);
    if (tableCatalogue.isTable(tableName)){
        // it is table
        Table table = *tableCatalogue.getTable(tableName);
        this->tableName = tableName;
        this->columnCount = table.columnCount;
        uint maxRowCount = table.maxRowsPerBlock;
        vector<int> row(columnCount, 0);
        this->rows.assign(maxRowCount, row);

        ifstream fin(pageName, ios::in);
        this->rowCount = table.rowsPerBlockCount[pageIndex];
        int number;
        for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
        {
            for (int columnCounter = 0; columnCounter < columnCount; columnCounter++)
            {
                fin >> number;
                this->rows[rowCounter][columnCounter] = number;
            }
        }
        fin.close();
    }
    else if (matrixCatalogue.isMatrix(tableName)){
        // it is a matrix
        logger.log("ACHHA MATRIX HAI");
        Matrix matrix = *matrixCatalogue.getMatrix(tableName);
        this->MatrixName = tableName;
        this->tableName = tableName;
        this->columnCount = MAX_COLS_MATRIX;
        uint maxRowCount = MAX_ROWS_MATRIX;
        uint maxColCount = MAX_COLS_MATRIX;
        vector<int> row(columnCount, 0);
        this->rows.assign(maxRowCount, row);
        logger.log("OK");
        logger.log(this->pageName);
        ifstream fin(this->pageName, ios::in);
        this->rowCount = MAX_ROWS_MATRIX;
        logger.log(to_string(this->rowCount));
        logger.log(to_string(this->columnCount));
        int number;
        for (uint rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
        {
            for (int columnCounter = 0; columnCounter < columnCount; columnCounter++)
            {
                fin >> number;
                logger.log("AARA");
                logger.log(to_string(number));
                this->rows[rowCounter][columnCounter] = number;
            }
        }
        fin.close();
    }
}

/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<int> Page::getRow(int rowIndex)
{
    logger.log("Page::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->rowCount)
        return result;
    return this->rows[rowIndex];
}

Page::Page(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("Page::Page2");
    this->MatrixName = tableName; // matrix
    this->tableName = tableName; // table
    this->pageIndex = pageIndex;
    this->rows = rows;
    this->rowCount = rowCount;
    this->columnCount = rows[0].size();
    this->pageName = "../data/temp/"+this->tableName + "_Page" + to_string(pageIndex);
    logger.log(this->pageName);
}

/**
 * @brief writes current page contents to file.
 * 
 */
void Page::writePage()
{
    logger.log("Page::writePage");
    ofstream fout(this->pageName, ios::trunc);
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        logger.log(to_string(rowCounter));
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            logger.log(to_string(columnCounter));
            if (columnCounter != 0)
                fout << " ";
            fout << this->rows[rowCounter][columnCounter];
        }
        fout << endl;
    }
    fout.close();
    logger.log("HO GAYA");
}

/**
 * @brief writes current page contents to file.
 * 
 */
void Page::swapElements()
{
    logger.log("Page::swapElements");
    logger.log(this->pageName);
    logger.log(to_string(this->rowCount));
    logger.log(to_string(this->columnCount));
    ofstream fout(this->pageName, ios::trunc);
    vector<int> row(this->rowCount, -1);
    vector<vector<int>> temp(this->columnCount, row);    
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        logger.log(to_string(rowCounter));
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            logger.log(to_string(columnCounter));
            if (columnCounter != 0)
                fout << " ";
            fout << this->rows[columnCounter][rowCounter];
            temp[rowCounter][columnCounter] = this->rows[columnCounter][rowCounter];
        }
        fout << endl;
    }
    fout.close();
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        logger.log(to_string(rowCounter));
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            logger.log(to_string(columnCounter));
            this->rows[rowCounter][columnCounter] = temp[rowCounter][columnCounter];
        }
    }
    logger.log("HO GAYA SWAP");
}

/**
 * @brief writes current page contents to a specified file.
 * 
 */
void Page::writeToPage(vector<vector<int>> matrix)
{
    logger.log("Page::writeToPage");
    logger.log((pageName));
    ofstream fout(this->pageName, ios::trunc);
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        logger.log(to_string(rowCounter));
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            logger.log(to_string(columnCounter));
            if (columnCounter != 0)
                fout << " ";
            fout << matrix[rowCounter][columnCounter];
            this->rows[rowCounter][columnCounter] = matrix[rowCounter][columnCounter];
        }
        fout << endl;
    }
    fout.close();
    logger.log("HO GAYA SWAP");
}

// Page 0 -> [2,0,0] "PAge0" "Kundli0" ../temp/Page_0->[2,0,0]
// Page 1 -> [1,0,0] "Page1" "Kundli1" ../temp/Page_1->[1,0,0]