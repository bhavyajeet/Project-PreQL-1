#include "matrix.h"

/**
 * @brief The TableCatalogue acts like an index of tables existing in the
 * system. Everytime a table is added(removed) to(from) the system, it needs to
 * be added(removed) to(from) the tableCatalogue. 
 *
 */
class MatrixCatalogue
{

    unordered_map<string, Matrix*> matrices;

public:
    MatrixCatalogue() {}
    void insertMatrix(Matrix* Matrix);
    void deleteMatrix(string MatrixName);
    Matrix* getMatrix(string MatrixName);
    bool isMatrix(string MatrixName);
    bool isColumnFromMatrix(string columnName, string MatrixName);
    void print();
    ~MatrixCatalogue();
};