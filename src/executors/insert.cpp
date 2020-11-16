#include "global.h"
/**
 * @brief 
 * SYNTAX: INSERT
 */
bool syntacticParseINSERT()
{
    logger.log("syntacticParseINSERT");
    cout << "INSERT syntactic parser\n";
    cout << tokenizedQuery.size() << endl << endl; 
    // if (tokenizedQuery.size() != 5)
    // {
    //     cout << "SYNTAX ERROR" << endl;
    //     return false;
    // }
    parsedQuery.queryType = INSERT;
    parsedQuery.insertRelationName = tokenizedQuery[2];
    string str = tokenizedQuery[4];
    // vector<int> v;
    parsedQuery.insertValues.clear();
    // stringstream ss(str);
    for (int stp=0;stp<tokenizedQuery.size()-4;stp++) {
        // string substr;
        // cout << tokenizedQuery[stp+4]<<endl;;

        // getline(ss, substr, ',');
        int val1 = stoi(tokenizedQuery[stp+4]);
        parsedQuery.insertValues.push_back(val1);
    }
 
    // for (size_t i = 0; i < parsedQuery.insertValues.size(); i++)
    //     cout << parsedQuery.insertValues[i] << endl;
    //     cout << parsedQuery.insertValues.size() << endl;
    return true;
}

bool semanticParseINSERT()
{
    logger.log("semanticParseINSERT");
    cout << "INSERT semantic parser";

    if (!tableCatalogue.isTable(parsedQuery.insertRelationName))
    {
        cout << "SEMANTIC ERROR: Insert relation does not exist" << endl;
        return false;
    }
    Table table = *tableCatalogue.getTable(parsedQuery.insertRelationName);
    if (table.columnCount != parsedQuery.insertValues.size()){
        cout << "SEMANTIC ERROR: values do not match number of columns" << endl;
        return false;
    }


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
    Table *  table = tableCatalogue.getTable(parsedQuery.insertRelationName);
    cout << table ->indexedColumn << " wtf ?? " << endl;
    table->insertRow(parsedQuery.insertValues);
    // insertbtree()
    // inserthash()
    cout << "I Will Execute Insert One Day";
    cout << endl << "That day is here" << endl;
}