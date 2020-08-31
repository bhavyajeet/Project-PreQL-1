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
    return true;
}

void executeTRANSPOSE()
{
    logger.log("executeTRANSPOSE");
    Matrix matrix = *(matrixCatalogue.getMatrix(parsedQuery.transposeRelationName));
    matrix.transpose();
}