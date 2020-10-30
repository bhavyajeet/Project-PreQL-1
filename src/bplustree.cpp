#include "global.h"

bplusTree::bplusTree()
{
    logger.log("bplusTree::bplusTree");
}

bplusTree::bplusTree(string tableName, string fanOut, int rowCount){
    logger.log("bplusTree::bplusTreeInitialization");
    for (int i = 0; i < rowCount; i++)
    {
        logger.log("AAA");
    }   
}