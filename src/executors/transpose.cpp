#include "global.h"
/**
 * @brief 
 * SYNTAX: TRANSPOSE MATRIX_NAME
 */
bool syntacticParseTRANSPOSE()
{
    logger.log("syntacticParseTRANSPOSE");
    parsedQuery.queryType = TRANSPOSE;
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
}