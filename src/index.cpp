#include "global.h"

/**
 * @brief Construct a new Index:: Index object
 *
 */
Index::Index()
{
    logger.log("Index::Index");
}

/**
 * @brief Construct a new Index:: Index object used in the case where the data
 * file is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param MatrixName 
 */
Index::Index(string IndexName)
{
    logger.log("Index::Index");
    this->sourceFileName = "../data/index" + MatrixName + ".csv";
    this->IndexName = IndexName;
}

/**
 * @brief Construct a new Matrix:: Matrix object used when an assignment command
 * is encountered. To create the Matrix object both the Matrix name and the
 * columns the Matrix holds should be specified.
 *
 * @param MatrixName 
 * @param columns 
 */
Matrix::Matrix(string IndexName, vector<string> columns)
{
    logger.log("Index::Index");
    this->sourceFileName = "../data/temp/index" + MatrixName + ".csv";
    this->IndexName = IndexName;
    this->columns = columns;
    this->columnCount = columns.size();
    this->maxRowsPerBlock = (uint)(MAX_ROWS_MATRIX);
    this->maxColumnsPerBlock = (uint)(MAX_COLS_MATRIX);
    this->writeRow<string>(columns);
}