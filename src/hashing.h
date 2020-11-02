
/**
 * @brief The BufferManager is responsible for reading pages to the main memory.
 * Recall that large files are broken and stored as blocks in the hard disk. The
 * minimum amount of memory that can be read from the disk is a block whose size
 * is indicated by BLOCK_SIZE. within this system we simulate blocks by
 * splitting and storing the file as multiple files each of one BLOCK_SIZE,
 * although this isn't traditionally how it's done. You can alternatively just
 * random access to the point where a block begins within the same
 * file. In this system we assume that the the sizes of blocks and pages are the
 * same. 
 * 
 * <p>
 * The buffer can hold multiple pages quantified by BLOCK_COUNT. The
 * buffer manager follows the FIFO replacement policy i.e. the first block to be
 * read in is replaced by the new incoming block. This replacement policy should
 * be transparent to the executors i.e. the executor should not know if a block
 * was previously present in the buffer or was read in from the disk. 
 * </p>
 *
 */
typedef struct bucket{
    int data;
    string tableName; // which table it belongs to
    int pagePtr; // which page it belongs to
    int rowPtr;  // which row it belongs to within the page
    struct bucket* next;
    struct bucket* prev;
}bt;

  
class hashing 
{ 
    
public: 
    int BUCKET;    // No. of buckets 
  
    // Pointer to an array containing buckets 
    vector <bt*> bucks; 
    int pointerBucket;
    string tableName;

    // inserts a key into hash table 
    void insertItem(int key, int pagePtr, int rowPtr);
  
    // deletes a key from hash table 
    void deleteItem(int key); 
  
    // hash function to map values to key 
    int hashFunction(int x) { 
        return (x % BUCKET); 
    } 

    hashing();
    hashing(string tableName, int b,int rowCount, int indexedColumnNumber );
  
    void displayHash(); 
}; 
