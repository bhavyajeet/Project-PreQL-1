#include "global.h"

/**
 * @brief 
 * SYNTAX: EXPORT <relation_name> 
 */

bool syntacticParseEXPORT()
{
    logger.log("syntacticParseEXPORT");
    if (tokenizedQuery.size() != 2)
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = EXPORT;
    parsedQuery.exportRelationName = tokenizedQuery[1];
    return true;
}

bool semanticParseEXPORT()
{
    logger.log("semanticParseEXPORT");
    //Table should exist
    if (tableCatalogue.isTable(parsedQuery.exportRelationName) || matrixCatalogue.isMatrix(parsedQuery.exportRelationName))
    {
        return true;
    }
    cout << "SEMANTIC ERROR: No such relation exists" << endl;
    return false;
}

void executeEXPORT()
{
    logger.log("executeEXPORT");
    if (tableCatalogue.isTable(parsedQuery.exportRelationName))
    {
        // EXPORT TABLE
        Table* table = tableCatalogue.getTable(parsedQuery.exportRelationName);
        table->makePermanent();            
    }
    else if(matrixCatalogue.isMatrix(parsedQuery.exportRelationName))
    {
        // EXPORT MATRIX
        Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.exportRelationName);
        matrix->makePermanent();            

    }
    return;
}