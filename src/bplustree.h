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

class Node {
    /*
		Generally size of the this node should be equal to the block size. Which will limit the number of disk access and increase the accesssing time.
		Intermediate nodes only hold the Tree pointers which is of considerably small size(so they can hold more Tree pointers) and only Leaf nodes hold
		the data pointer directly to the disc.
		IMPORTANT := All the data has to be present in the leaf node
	*/
   public:
    bool isLeaf;
    vector<int> keys;
    Node();
    //Node* ptr2parent; //Pointer to go to parent node CANNOT USE check https://stackoverflow.com/questions/57831014/why-we-are-not-saving-the-parent-pointer-in-b-tree-for-easy-upward-traversal-in
    Node* ptr2next;              //Pointer to connect next node for leaf nodes
    union ptr {                  //to make memory efficient Node
        vector<Node*> ptr2Tree;  //Array of pointers to Children sub-trees for intermediate Nodes
        vector< pair<int,int> > dataPtr;   // Data-Pointer for the leaf node

        ptr();   // To remove the error !?
        ~ptr();  // To remove the error !?
    } ptr2TreeOrData;

    friend class bplusTree;  // to access private members of the Node and hold the encapsulation concept
        
};

class bplusTree
{
public:
    int fanOut;
    int maxIntChildLimit;                                   // Limiting #of children for internal Nodes!
    int maxLeafNodeLimit;                                   // Limiting #of nodes for leaf Nodes!!!
    Node* root;                                             // Pointer to the B+ Tree root

    bplusTree();
    // bplusTree(int degreeInternal, int degreeLeaf);
    bplusTree(string tableName, string fanOut, int rowCount, int indexedColumnNumber);
    void insertInternal(int x, Node** cursor, Node** child);  //Insert x from child in cursor(parent)
    void insert(int key, int rowPtr, int pagePtr) ;  //in Leaf Node
    Node** findParent(Node* cursor, Node* child);
    // Node* firstLeftNode(Node* cursor);
    // Node* getRoot();
    // int getMaxIntChildLimit();
    // int getMaxLeafNodeLimit();
    // void setRoot(Node *);
    // void display(Node* cursor);
    // void seqDisplay(Node* cursor);
    // void search(int key);
    // void insert(int key, FILE* filePtr);
    // void removeKey(int key);
    // void removeInternal(int x, Node* cursor, Node* child);

};
};