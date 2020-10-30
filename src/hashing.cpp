#include "global.h"

hashing::hashing()
{
    logger.log("hashing::hashing");
}

hashing::hashing(string tableName, string fanOut, int rowCount, int indexedColumnNumber){
    logger.log("hashing::hashingInitialization");
    for (int i = 0; i < rowCount; i++)
    {
        logger.log("AAA");
    }
}
