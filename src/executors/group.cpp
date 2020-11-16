#include "global.h"
/**
 * @brief 
 * SYNTAX: R <- SELECT column_name bin_op [column_name | int_literal] FROM relation_name
 */
bool syntacticParseGROUP()
{
    for (auto k: tokenizedQuery){
        cout << k <<" ";
    }
    cout << endl;
    logger.log("syntacticParseGROUP");
    if (tokenizedQuery.size() != 10 || tokenizedQuery[5] != "FROM" || tokenizedQuery[7] != "RETURN")
    {
        cout << "SYNTAC ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = GROUP;
    parsedQuery.groupResultRelationName = tokenizedQuery[0];
    parsedQuery.groupColumnName = tokenizedQuery[4];
    parsedQuery.groupRelationName = tokenizedQuery[6];
    parsedQuery.groupOperationName = tokenizedQuery[8];
    if (tokenizedQuery[9]!=""){
       tokenizedQuery[9].pop_back();
       parsedQuery.groupOperationColumn = tokenizedQuery[9];
       cout << parsedQuery.groupOperationColumn << endl;
    }
    else {
        cout << "SYNTAC ERROR" << endl;
        return false;
    }



    // string binaryOperator = tokenizedQuery[4];
    // if (binaryOperator == "<")
    //     parsedQuery.selectionBinaryOperator = LESS_THAN;
    // else if (binaryOperator == ">")
    //     parsedQuery.selectionBinaryOperator = GREATER_THAN;
    // else if (binaryOperator == ">=" || binaryOperator == "=>")
    //     parsedQuery.selectionBinaryOperator = GEQ;
    // else if (binaryOperator == "<=" || binaryOperator == "=<")
    //     parsedQuery.selectionBinaryOperator = LEQ;
    // else if (binaryOperator == "==")
    //     parsedQuery.selectionBinaryOperator = EQUAL;
    // else if (binaryOperator == "!=")
    //     parsedQuery.selectionBinaryOperator = NOT_EQUAL;
    // else
    // {
    //     cout << "SYNTAC ERROR" << endl;
    //     return false;
    // }
    // regex numeric("[-]?[0-9]+");
    // string secondArgument = tokenizedQuery[5];
    // if (regex_match(secondArgument, numeric))
    // {
    //     parsedQuery.selectType = INT_LITERAL;
    //     parsedQuery.selectionIntLiteral = stoi(secondArgument);
    // }
    // else
    // {
    //     parsedQuery.selectType = COLUMN;
    //     parsedQuery.selectionSecondColumnName = secondArgument;
    // }
    return true;
}

bool semanticParseGROUP()
{
    logger.log("semanticParseGROUP");

    if (tableCatalogue.isTable(parsedQuery.groupResultRelationName))
    {
        cout << "SEMANTIC ERROR: Resultant relation already exists" << endl;
        return false;
    }

    if (!tableCatalogue.isTable(parsedQuery.groupRelationName))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
        return false;
    }

    if (!tableCatalogue.isColumnFromTable(parsedQuery.groupColumnName, parsedQuery.groupRelationName))
    {
        cout << "SEMANTIC ERROR: Column "  << parsedQuery.groupColumnName  <<" doesn't exist in relation" << endl;
        return false;
    }
    if (!tableCatalogue.isColumnFromTable(parsedQuery.groupOperationColumn, parsedQuery.groupRelationName))
    {
        cout << "SEMANTIC ERROR: Column "  << parsedQuery.groupOperationColumn  <<" doesn't exist in relation" << endl;
        return false;
    }

    return true;
}


/*
bool evaluateBinOp(int value1, int value2, BinaryOperator binaryOperator)
{
    switch (binaryOperator)
    {
    case LESS_THAN:
        return (value1 < value2);
    case GREATER_THAN:
        return (value1 > value2);
    case LEQ:
        return (value1 <= value2);
    case GEQ:
        return (value1 >= value2);
    case EQUAL:
        return (value1 == value2);
    case NOT_EQUAL:
        return (value1 != value2);
    default:
        return false;
    }
}
*/

void executeGROUP()
{
    logger.log("executeSELECTION");

    if (parsedQuery.groupOperationName=="MAX"){
        cout << "gotta find group max" << endl;
    }

    if (parsedQuery.groupOperationName=="MIN"){
        cout << "gotta find group min" << endl;
    }

    if (parsedQuery.groupOperationName=="SUM"){
        cout << "gotta find group sum" << endl;
    }

    if (parsedQuery.groupOperationName=="AVG"){
        cout << "gotta find group avg" << endl;
    }


// /*
    Table tableOG = *tableCatalogue.getTable(parsedQuery.groupRelationName);
    bool indexed = tableOG.indexed;
    IndexingStrategy indexingStrategy = tableOG.indexingStrategy;
    string thirdParam   = tableOG.thirdParam;
    string indexedColumn = tableOG.indexedColumn;

    tableOG.sortNoIndex(parsedQuery.groupColumnName, "G_sort"+tableOG.tableName);

    Table table = *tableCatalogue.getTable("G_sort"+parsedQuery.groupRelationName);
    int groupColumnIndex = table.getColumnIndex(parsedQuery.groupColumnName);
    int oprColumnIndex = table.getColumnIndex(parsedQuery.groupOperationColumn);
    // int oprColumnIndex1 = tableOG.getColumnIndex(parsedQuery.groupOperationColumn);
    cout << "oprcolumn " << oprColumnIndex << " name :" << parsedQuery.groupOperationColumn << endl;
    // cout << "oprcolumn " << oprColumnIndex1 << " name :" << parsedQuery.groupOperationColumn << endl;
    
    vector <string> cols;
    cols.push_back(parsedQuery.groupColumnName);
    cols.push_back(parsedQuery.groupOperationName+ parsedQuery.groupOperationColumn);
    Table* resultantTable = new Table(parsedQuery.groupResultRelationName, cols);
    
    Cursor cursor = table.getCursor();
    vector<int> row = cursor.getNext();
    



    if (parsedQuery.groupOperationName=="MAX"){
        
        int curgrpval;
        int result = -1;
        bool  flaggy =0;

        if (!row.empty()){
            curgrpval = row[groupColumnIndex];
            result = row[oprColumnIndex];
            flaggy=1;
        }
        else {
            cout << "empty table" << endl;
        }


        while (!row.empty()){
            if (curgrpval == row[groupColumnIndex]){
                
                if (row[oprColumnIndex] > result){
                    result = row[oprColumnIndex];
                }

                cout << "cont row: ";
                for (auto x : row){
                    cout << x << " ";
                }
                cout << endl;
                row = cursor.getNext();
            }
            else {
                vector <int> toWrite ; 
                toWrite.push_back(curgrpval);
                toWrite.push_back(result);
                
                cout << "writing row " << curgrpval << " " << result << endl;
                resultantTable->writeRow(toWrite);
                
                curgrpval = row[groupColumnIndex];
                result = row[oprColumnIndex];
                cout << "first group row: ";
                for (auto x : row){
                    cout << x << " ";
                }
                cout << endl;
                row = cursor.getNext();
            }
        }
        
        if (flaggy){
            vector <int> toWrite ; 
            toWrite.push_back(curgrpval);
            toWrite.push_back(result);
            
            resultantTable->writeRow(toWrite);
        }
        
    }



    else if (parsedQuery.groupOperationName=="MIN"){
        
        int curgrpval;
        int result = -1;
        bool  flaggy =0;

        if (!row.empty()){
            curgrpval = row[groupColumnIndex];
            result = row[oprColumnIndex];
            flaggy=1;
        }
        else {
            cout << "empty table" << endl;
        }


        while (!row.empty()){
            if (curgrpval == row[groupColumnIndex]){
                
                if (row[oprColumnIndex] < result){
                    result = row[oprColumnIndex];
                }

                cout << "cont row: ";
                for (auto x : row){
                    cout << x << " ";
                }
                cout << endl;
                row = cursor.getNext();
            }
            else {
                vector <int> toWrite ; 
                toWrite.push_back(curgrpval);
                toWrite.push_back(result);
                
                cout << "writing row " << curgrpval << " " << result << endl;
                resultantTable->writeRow(toWrite);
                
                curgrpval = row[groupColumnIndex];
                result = row[oprColumnIndex];
                cout << "first group row: ";
                for (auto x : row){
                    cout << x << " ";
                }
                cout << endl;
                row = cursor.getNext();
            }
        }
        
        if (flaggy){
            vector <int> toWrite ; 
            toWrite.push_back(curgrpval);
            toWrite.push_back(result);
            
            resultantTable->writeRow(toWrite);
        }
        
    }



    else if (parsedQuery.groupOperationName=="SUM"){
        
        int curgrpval;
        int result = -1;
        bool  flaggy =0;

        if (!row.empty()){
            curgrpval = row[groupColumnIndex];
            result = 0;
            flaggy=1;
        }
        else {
            cout << "empty table" << endl;
        }


        while (!row.empty()){
            if (curgrpval == row[groupColumnIndex]){
                
                
                result += row[oprColumnIndex];
                

                cout << "cont row: ";
                for (auto x : row){
                    cout << x << " ";
                }
                cout << endl;
                row = cursor.getNext();
            }
            else {
                vector <int> toWrite ; 
                toWrite.push_back(curgrpval);
                toWrite.push_back(result);
                
                cout << "writing row " << curgrpval << " " << result << endl;

                resultantTable->writeRow(toWrite);
                
                curgrpval = row[groupColumnIndex];
                result = row[oprColumnIndex];
                cout << "first group row: ";
                for (auto x : row){
                    cout << x << " ";
                }
                cout << endl;
                row = cursor.getNext();
            }
        }
        
        if (flaggy){
            vector <int> toWrite ; 
            toWrite.push_back(curgrpval);
            toWrite.push_back(result);
            
            resultantTable->writeRow(toWrite);
        }
        
    }



    else if (parsedQuery.groupOperationName=="AVG"){
        
        int curgrpval;
        int result = -1;
        int count;
        bool  flaggy =0;

        if (!row.empty()){
            curgrpval = row[groupColumnIndex];
            result = 0;
            count = 0; 
            flaggy=1;
        }
        else {
            cout << "empty table" << endl;
        }


        while (!row.empty()){
            if (curgrpval == row[groupColumnIndex]){
                
                
                result += row[oprColumnIndex];
                count++;

                cout << "cont row: ";
                for (auto x : row){
                    cout << x << " ";
                }
                cout << endl;
                row = cursor.getNext();
            }
            else {
                vector <int> toWrite ; 
                toWrite.push_back(curgrpval);
                toWrite.push_back(result/count);
                
                cout << "writing row " << curgrpval << " " << result << endl;

                resultantTable->writeRow(toWrite);
                
                curgrpval = row[groupColumnIndex];
                result = row[oprColumnIndex];
                count = 1 ;

                cout << "first group row: ";
                for (auto x : row){
                    cout << x << " ";
                }
                cout << endl;
                row = cursor.getNext();
            }
        }
        
        if (flaggy){
            vector <int> toWrite ; 
            toWrite.push_back(curgrpval);
            toWrite.push_back(result/count);
            
            resultantTable->writeRow(toWrite);
        }
        
    }


    

            


    // int secondColumnIndex;
    // if (parsedQuery.selectType == COLUMN)
    // while (!row.empty())
    // {

    //     int value1 = row[firstColumnIndex];
    //     int value2;
    //     if (parsedQuery.selectType == INT_LITERAL)
    //         value2 = parsedQuery.selectionIntLiteral;
    //     else
    //         value2 = row[secondColumnIndex];
    //     if (evaluateBinOp(value1, value2, parsedQuery.selectionBinaryOperator))
    //         resultantTable->writeRow<int>(row);
    //     row = cursor.getNext();
    // }
    if(resultantTable->blockify())
        tableCatalogue.insertTable(resultantTable);
    else{
        cout<<"Empty Table"<<endl;
        delete resultantTable;
    }

    if(indexed){
        Table * tabl = tableCatalogue.getTable(parsedQuery.groupRelationName);
        if(tabl){
            tabl->indexTable(indexedColumn,indexingStrategy,thirdParam);
        }
    }
// */
    return;
}