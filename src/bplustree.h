namespace std
{
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



// BP node
class Node {
  friend class bplusTree;

   public:
  bool IS_LEAF;
  vector < pair<int,int> > dataPtr;
  int *key, size;
  int *rowPtr;
  int *pagePtr;
  Node **ptr;

  Node();
};

// BP tree
class bplusTree {
  Node *root;
  void insertInternal(int key, int pagePtr, int rowPtr, Node * nd1, Node *nd2);
  Node *findParent(Node *nd1, Node *nd2);
  int MAX;

   public:
   string tableName;
  bplusTree();

  void removeKey(int x);
  void removeInternal(int x, Node* cursor, Node* child);
  bplusTree(string tableName, string fanOut, int rowCount, int indexedColumn);
  pair<int,int>  search(int key);
  void searchAndUpdate(int x, int pagePtr, int rowPtr);
  void insert(int x, int pagePtr, int rowPtr);
  void display(Node * nd);
  Node *getRoot();
};



};