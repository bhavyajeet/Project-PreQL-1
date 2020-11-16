#include "global.h"
/**
 * @brief 
 * SYNTAX: Alter table 
 */
bool syntacticParseALTER()
{
    cout << "parsing AL syntax " << endl;
    logger.log("syntacticParseALTER");
    if (tokenizedQuery.size() != 6 || tokenizedQuery[1] != "TABLE" || tokenizedQuery[4] != "COLUMN")
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = ALTER;
    parsedQuery.alterRelationName = tokenizedQuery[2];
    parsedQuery.alterOperation = tokenizedQuery[3];
    parsedQuery.alterColumnName = tokenizedQuery[5];
    return true;
}

bool semanticParseALTER()
{
    logger.log("semanticParseALTER");
    cout << " alter semantic" << endl;
    if (!tableCatalogue.isTable(parsedQuery.alterRelationName))
    {
        cout << "SEMANTIC ERROR ALTER: Relation doesn't exist" << endl;
        return false;
    }

    if (parsedQuery.alterOperation == "ADD")
    {
        if (tableCatalogue.isColumnFromTable(parsedQuery.alterColumnName, parsedQuery.alterRelationName))
        {
            cout << "SEMANTIC ERROR: Column with name already exists" << endl;
            return false;
        }
    }

    else if (parsedQuery.alterOperation == "DELETE")
    {
        if (!tableCatalogue.isColumnFromTable(parsedQuery.alterColumnName, parsedQuery.alterRelationName))
        {
            cout << "SEMANTIC ERROR: Column "<< parsedQuery.alterColumnName <<" doesn't exist in relation" << endl;
            return false;
        }
    }

    else
    {
        cout << "SEMANTIC ERROR: INVALID OPERATION" << endl;
        return false;
    }

    return true;
}

void executeALTER()
{
    logger.log("executeALTER");
    cout <<"executeALTER"<< endl;
    Table *table = tableCatalogue.getTable(parsedQuery.alterRelationName);
    bool indexed = table->indexed;
    IndexingStrategy indexingStrategy = table->indexingStrategy;
    string thirdParam   = table->thirdParam;
    string indexedColumn = table->indexedColumn;
    if (parsedQuery.alterOperation == "ADD")
    {
        cout <<"adding"<< endl;
        table->addCol(parsedQuery.alterColumnName);
    }
    else if (parsedQuery.alterOperation == "DELETE")
    {
        table->deleteCol(parsedQuery.alterColumnName);
    }
    cout << "will alter table " << endl;
    if(indexed){
        Table * tabl = tableCatalogue.getTable(parsedQuery.alterRelationName);
        if(tabl){
            tabl->indexTable(indexedColumn,indexingStrategy,thirdParam);
        }
    }
    return;
}