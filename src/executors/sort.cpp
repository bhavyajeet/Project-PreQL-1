#include"global.h"
/**
 * @brief File contains method to process SORT commands.
 * 
 * syntax:
 * R <- SORT relation_name BY column_name IN sorting_order
 * 
 * sorting_order = ASC | DESC 
 */
bool syntacticParseSORT(){
    logger.log("syntacticParseSORT");
    if((tokenizedQuery.size()!= 8 && tokenizedQuery.size()!= 10  ) || tokenizedQuery[4] != "BY" || tokenizedQuery[6] != "IN"){
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
    parsedQuery.queryType = SORT;
    parsedQuery.sortResultRelationName = tokenizedQuery[0];
    parsedQuery.sortColumnName = tokenizedQuery[5];
    parsedQuery.sortRelationName = tokenizedQuery[3];
    string sortingStrategy = tokenizedQuery[7];
    if (tokenizedQuery.size()== 10){
        parsedQuery.sortBufferSize = stoi(tokenizedQuery[9]);
    }
    if(sortingStrategy == "ASC")
        parsedQuery.sortingStrategy = ASC;
    else if(sortingStrategy == "DESC")
        parsedQuery.sortingStrategy = DESC;
    else{
        cout<<"SYNTAX ERROR"<<endl;
        return false;
    }
    return true;
}

bool semanticParseSORT(){
    logger.log("semanticParseSORT");

    cout << "semanticParseSORT";

    cout << parsedQuery.sortResultRelationName<< endl;
    cout << parsedQuery.sortColumnName<< endl;

    if(tableCatalogue.isTable(parsedQuery.sortResultRelationName)){
        cout<<"SEMANTIC ERROR: Resultant relation already exists"<<endl;
        return false;
    }

    if(!tableCatalogue.isTable(parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Relation " << parsedQuery.sortRelationName << " doesn't exist"<<endl;
        return false;
    }

    if(!tableCatalogue.isColumnFromTable(parsedQuery.sortColumnName, parsedQuery.sortRelationName)){
        cout<<"SEMANTIC ERROR: Column doesn't exist in relation"<<endl;
        return false;
    }

    return true;
}

void executeSORT(){
    logger.log("executeSORT");
    cout << "Exeting"<< endl;
    Table* tableToSort = tableCatalogue.getTable(parsedQuery.sortRelationName);

    if (parsedQuery.sortingStrategy == ASC)
    { 
       tableToSort->sortNoIndex(parsedQuery.sortColumnName,parsedQuery.sortResultRelationName,1,parsedQuery.sortBufferSize);
    }
    else 
    {
        tableToSort->sortDesc(parsedQuery.sortColumnName,parsedQuery.sortResultRelationName,1,parsedQuery.sortBufferSize);
    }

    return;
}