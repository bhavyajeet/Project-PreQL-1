#include "global.h"

void MatrixCatalogue::insertMatrix(Matrix* Matrix)
{
    logger.log("MatrixCatalogue::~insertMatrix"); 
    this->matrices[Matrix->MatrixName] = Matrix;
}
void MatrixCatalogue::deleteMatrix(string MatrixName)
{
    logger.log("MatrixCatalogue::deleteMatrix"); 
    this->matrices[MatrixName]->unload();
    delete this->matrices[MatrixName];
    this->matrices.erase(MatrixName);
}
Matrix* MatrixCatalogue::getMatrix(string MatrixName)
{
    logger.log("MatrixCatalogue::getMatrix"); 
    Matrix *Matrix = this->matrices[MatrixName];
    return Matrix;
}
bool MatrixCatalogue::isMatrix(string MatrixName)
{
    logger.log("MatrixCatalogue::isMatrix"); 
    if (this->matrices.count(MatrixName))
        return true;
    return false;
}

bool MatrixCatalogue::isColumnFromMatrix(string columnName, string MatrixName)
{
    logger.log("MatrixCatalogue::isColumnFromMatrix"); 
    if (this->isMatrix(MatrixName))
    {
        Matrix* Matrix = this->getMatrix(MatrixName);
        if (Matrix->isColumn(columnName))
            return true;
    }
    return false;
}

void MatrixCatalogue::print()
{
    logger.log("MatrixCatalogue::print"); 
    cout << "\nRELATIONS" << endl;

    int rowCount = 0;
    for (auto rel : this->matrices)
    {
        cout << rel.first << endl;
        rowCount++;
    }
    printRowCount(rowCount);
}

MatrixCatalogue::~MatrixCatalogue(){
    logger.log("MatrixCatalogue::~MatrixCatalogue"); 
    for(auto matrix: this->matrices){
        matrix.second->unload();
        delete matrix.second;
    }
}
