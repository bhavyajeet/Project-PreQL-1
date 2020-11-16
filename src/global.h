#include"executor.h"

#ifndef PROTECTION
#define PROTECTION "PLEASE_USE_PROTECTION"
extern float BLOCK_SIZE;
extern int MAX_ROWS_MATRIX;
extern int MAX_COLS_MATRIX;
extern uint BLOCK_COUNT;
extern uint PRINT_COUNT;
extern vector<string> tokenizedQuery;
extern ParsedQuery parsedQuery;
extern TableCatalogue tableCatalogue;
extern MatrixCatalogue matrixCatalogue;
extern BufferManager bufferManager;
#endif // !
