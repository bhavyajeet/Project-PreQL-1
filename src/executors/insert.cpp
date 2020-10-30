#include "global.h"
/**
 * @brief 
 * SYNTAX: TRANSPOSE MATRIX_NAME
 */
bool syntacticParseINSERT()
{
    logger.log("syntacticParseINSERT");
    cout << "INSERT syntactic parser";
    // if (tokenizedQuery.size() != 2)
    // {
    //     cout << "SYNTAX ERROR" << endl;
    //     return false;
    // }
    parsedQuery.queryType = INSERT;
    parsedQuery.transposeRelationName = tokenizedQuery[1];
    return true;
}

bool semanticParseINSERT()
{
    logger.log("semanticParseINSERT");
    cout << "INSERT semantic parser";
    // if(!matrixCatalogue.isMatrix(parsedQuery.transposeRelationName)){
    //     cout << "MATRIX DOES NOT EXIST" << endl;
    //     return false;        
    // }
    // if (!isFileExists(parsedQuery.transposeRelationName))
    // {
    //     cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
    //     return false;
    // }

    return true;
}

void executeINSERT()
{
    logger.log("executeINSERT");
    cout << "I Will Execute Insert One Day";
}