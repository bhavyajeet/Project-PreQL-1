#include "global.h"

/**
 * @brief 
 * SYNTAX: R <- DISTINCT relation_name
 */
bool syntacticParseDELETE()
{
    logger.log("syntacticParseDELETE");
    parsedQuery.queryType = DELETE;
    parsedQuery.deleteRelationName = tokenizedQuery[2];
    string str = tokenizedQuery[4];
    parsedQuery.deleteValues.clear();
    for (int stp=0;stp<tokenizedQuery.size()-4;stp++) {
        int val1 = stoi(tokenizedQuery[stp+4]);
        parsedQuery.deleteValues.push_back(val1);
    }
    return true;
}

bool semanticParseDELETE()
{
    logger.log("semanticParseDELETE");
    //The resultant table shouldn't exist and the table argument should
    if (!tableCatalogue.isTable(parsedQuery.deleteRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation doesnt exist" << endl;
        return false;
    }
    return true;
}

void executeDELETE()
{
    logger.log("executeDELETE");
    cout << "MAI DELETE KARNE VALA HUN" << endl;
    for (int stp=0;stp<tokenizedQuery.size()-4;stp++) {
        int val1 = stoi(tokenizedQuery[stp+4]);
        cout << val1 << " " << endl;
    }
    cout << endl;
    Table * table = tableCatalogue.getTable(parsedQuery.deleteRelationName);
    int ret = table->deleteRow(parsedQuery.deleteValues);
    if(!ret){
        cout << "NO SUCH RELATION FOUND" << endl;
    }
    return;
}