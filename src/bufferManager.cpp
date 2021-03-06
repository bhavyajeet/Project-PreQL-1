#include "global.h"

BufferManager::BufferManager()
{
    logger.log("BufferManager::BufferManager");
}

/**
 * @brief Function called to read a page from the buffer manager. If the page is
 * not present in the pool, the page is read and then inserted into the pool.
 *
 * @param tableName 
 * @param pageIndex 
 * @return Page 
 */
Page BufferManager::getPage(string tableName, int pageIndex)
{
    // cout << " request for " << "../data/temp/"+tableName + "_Page" + to_string(pageIndex) << endl;
    logger.log("BufferManager::getPage");
    string pageName = "../data/temp/"+tableName + "_Page" + to_string(pageIndex);
    logger.log("searching");
    logger.log(pageName);
    if (this->inPool(pageName) && tableName.at(0) != '_' && tableName != "Y" )
    {
        // cout << "IN POOOL IN POOOL IN POOL "<< endl;;
        return this->getFromPool(pageName);
    }
    else
        {
            // cout << "mother board  \n";
            logger.log(tableName);
            logger.log("ANNARA");
            logger.log("TERERERE");
            return this->insertIntoPool(tableName, pageIndex);
        }
}

/**
 * @brief Checks to see if a page exists in the pool
 *
 * @param pageName 
 * @return true 
 * @return false 
 */
bool BufferManager::inPool(string pageName)
{
    logger.log("BufferManager::inPool");
    for (auto page : this->pages)
    {
        logger.log("OO");
        logger.log(page.pageName);
        // cout << page.pageName << endl;
        if (pageName == page.pageName)
            return true;
    }
    return false;
}

/**
 * @brief If the page is present in the pool, then this function returns the
 * page. Note that this function will fail if the page is not present in the
 * pool.
 *
 * @param pageName 
 * @return Page 
 */
Page BufferManager::getFromPool(string pageName)
{
    logger.log("BufferManager::getFromPool");
    for (auto page : this->pages)
        if (pageName == page.pageName)
        {
            // cout << "Buffer returning : " << page.pageName << endl;
            vector <int> tester = page.getRow(0);
            // for (auto x : tester){
            //     cout << x << " ";
            // } 
            // cout <<  "POOL puss" << endl; 
            return page;
        }
}

/**
 * @brief Inserts page indicated by tableName and pageIndex into pool. If the
 * pool is full, the pool ejects the oldest inserted page from the pool and adds
 * the current page at the end. It naturally follows a queue data structure. 
 *
 * @param tableName 
 * @param pageIndex 
 * @return Page 
 */
Page BufferManager::insertIntoPool(string tableName, int pageIndex)
{
    logger.log("BufferManager::insertIntoPool");
    Page page(tableName, pageIndex);
    cout << page.getRowCount() << "  " << tableName << "  " << pageIndex << endl ;
    if (this->pages.size() >= BLOCK_COUNT)
        pages.pop_front();
    pages.push_back(page);
    vector <int> tester = page.getRow(0);
    return page;
}

/**
 * @brief The buffer manager is also responsible for writing pages. This is
 * called when new tables are created using assignment statements.
 *
 * @param tableName 
 * @param pageIndex 
 * @param rows 
 * @param rowCount 
 */
void BufferManager::writePage(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("BufferManager::writePage");
    Page page(tableName, pageIndex, rows, rowCount);
    page.writePage();
}

/**
 * @brief The buffer manager is also responsible for updating pages. This is
 * called when new tables are created using assignment statements.
 *
 * @param tableName 
 * @param pageIndex 
 * @param rows 
 * @param rowCount 
 */
void BufferManager::updatePage(string pageName,Page newPage)
{
    cout << "trying to update page " << newPage.pageName << " with rows " << newPage.getRowCount() << endl;
    logger.log("BufferManager::updatePage");
    int x = 0;
    for (auto page : this->pages)
        {
            // cout << "|" << page.pageName << "|" << endl;
            if ("../data/temp/" + pageName == page.pageName)
            {
                    // cout << "ALLU ARJUN ANNAYA" << endl;;
                    this->pages[x] = newPage;
                    page = newPage;
            }
            x++;
        }
}


/**
 * @brief Function that unloads the pagr from bufferManager
 * 
 * @param  pageName
 * @return 
 */
void BufferManager::unloadPage(string pageName)
{
    cout << "trying to delete page " << pageName << " with rows " << pageName << endl;
    logger.log("BufferManager::removePage");
    int x = 0;
    for (auto page : this->pages)
        {
            // cout << "../data/temp/" + pageName<< " == "<< page.pageName << endl;
            if ( pageName == page.pageName)
            {
                cout << "UNLOADED" << endl;
                this->pages[x].pageName = "dummyPage";                 
            }
            x++;
        }
}


void BufferManager::unloadPages()
{
    cout << "trying to delete page "  << endl;
    logger.log("BufferManager::removePage");
    int x = 0;
    for (auto page : this->pages)
        {
            // cout << "../data/temp/" + pageName<< " == "<< page.pageName << endl;
            cout << "UNLOADED" << endl;
            this->pages[x].pageName = "dummyPage";                 
            x++;
        }
}


/**
 * @brief Deletes file names fileName
 *
 * @param fileName 
 */
void BufferManager::deleteFile(string fileName)
{
    
    if (remove(fileName.c_str()))
        logger.log("BufferManager::deleteFile: Err");
        else logger.log("BufferManager::deleteFile: Success");
}

/**
 * @brief Overloaded function that calls deleteFile(fileName) by constructing
 * the fileName from the tableName and pageIndex.
 *
 * @param tableName 
 * @param pageIndex 
 */
void BufferManager::deleteFile(string tableName, int pageIndex)
{
    logger.log("BufferManager::deleteFile");
    string fileName = "../data/temp/"+tableName + "_Page" + to_string(pageIndex);
    this->deleteFile(fileName);
}

//////////////////////////    MATRIX METHODS         /////////////////////////////
/**
 * @brief Inserts page indicated by matrixName and pageIndex into pool. If the
 * pool is full, the pool ejects the oldest inserted page from the pool and adds
 * the current page at the end. It naturally follows a queue data structure. 
 *
 * @param MatrixName 
 * @param pageIndex 
 * @return Page 
 */
Page BufferManager::insertMatrixIntoPool(string matrixName, int pageIndex)
{
    logger.log("BufferManager::insertMatrixIntoPool");
    Page page(matrixName, pageIndex);
    if (this->pages.size() >= BLOCK_COUNT)
        pages.pop_front();
    pages.push_back(page);
    return page;
}
