#include "global.h"
/**
 * @brief 
 * SYNTAX: TRANSPOSE MATRIX_NAME
 */
bool syntacticParseTRANSPOSE()
{
    logger.log("syntacticParseTRANSPOSE");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = TRANSPOSE;
    parsedQuery.transposeRelationName = tokenizedQuery[1];
    return true;
}

bool semanticParseTRANSPOSE()
{
    logger.log("semanticParseTRANSPOSE");
    if(!matrixCatalogue.isMatrix(parsedQuery.transposeRelationName)){
        cout << "MATRIX DOES NOT EXIST" << endl;
        return false;        
    }
    if (!isFileExists(parsedQuery.transposeRelationName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }

    return true;
}

void executeTRANSPOSE()
{
    logger.log("executeTRANSPOSE");
    Matrix matrix = *(matrixCatalogue.getMatrix(parsedQuery.transposeRelationName));
    matrix.transpose();
}