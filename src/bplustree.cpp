#include "global.h"

int MAAX ;

Node::Node() {
  key = new int[MAAX];
  pagePtr = new int[MAAX];
  rowPtr = new int[MAAX];
  ptr = new Node *[MAAX + 1];
}

bplusTree::bplusTree() {
  root = NULL;
}

bplusTree::bplusTree(string tableName, string fanOut, int rowCount, int indexedColumn) {
  this->MAX = stoi(fanOut);
  MAAX = this->MAX;
  this->tableName = tableName;
  root = NULL;
}


// Search operation
 pair<int,int> bplusTree::search(int x) {
  if (root == NULL) {
    cout << "Tree is empty\n";
  } else {
    Node *cursor = root;
    while (cursor->IS_LEAF == false) {
      for (int i = 0; i < cursor->size; i++) {
        if (x < cursor->key[i]) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    for (int i = 0; i < cursor->size; i++) {
      if (cursor->key[i] == x) {
        cout << "Found\n";
        return make_pair(cursor->pagePtr[i],cursor->rowPtr[i]);
      }
    }
    cout << "Not found\n";
  }
}

// Insert Operation
void bplusTree::insert(int x, int pagePtr, int rowPtr) {
    cout << "WHAT IS YOUR NAME?\t" << x <<  endl;
  if (root == NULL) {
      cout << "RERERERERERE" << endl;
    root = new Node;
    cout << "WINDI";
    root->key[0] = x;
    cout << "RANDI";
    root->rowPtr[0] = rowPtr;
    root->pagePtr[0] = pagePtr;
    root->IS_LEAF = true;
    root->size = 1;
    cout << "RARARARAR" << endl;
  } else {
    Node *cursor = root;
    Node *parent;
    while (cursor->IS_LEAF == false) {
      parent = cursor;
      for (int i = 0; i < cursor->size; i++) {
        if (x < cursor->key[i]) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    if (cursor->size < this->MAX) {
      int i = 0;
      while (x > cursor->key[i] && i < cursor->size)
        i++;
      for (int j = cursor->size; j > i; j--) {
        cursor->key[j] = cursor->key[j - 1];
        cursor->pagePtr[j] = cursor->pagePtr[j - 1];
        cursor->rowPtr[j] = cursor->rowPtr[j - 1];
      }
      cursor->key[i] = x;
    cursor->pagePtr[i] = pagePtr;
    cursor->rowPtr[i] = rowPtr;

      cursor->size++;
      cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
      cursor->ptr[cursor->size - 1] = NULL;
    } else {
        cout << "LAUWA ADAADFAF\t" << x  << endl;
      Node *newLeaf = new Node;
      int virtualNode[this->MAX + 1];
      int virtualPageNode[this->MAX + 1];
      int virtualRowNode[this->MAX + 1];
      for (int i = 0; i < this->MAX; i++) {
        virtualNode[i] = cursor->key[i];
        virtualPageNode[i] = cursor->pagePtr[i];
        virtualRowNode[i] = cursor->rowPtr[i];
      }
      cout << "AUR KAISA HAI BEY" << endl;
      int i = 0, j;
      while (x > virtualNode[i] && i < this->MAX)
        i++;
      for (int j = this->MAX + 1; j > i; j--) {
        virtualNode[j] = virtualNode[j - 1];
        virtualPageNode[j] = virtualPageNode[j - 1];
        virtualRowNode[j] = virtualRowNode[j - 1];
      }
      virtualNode[i] = x;
        virtualPageNode[i] = pagePtr;
        virtualRowNode[i] = rowPtr;

      newLeaf->IS_LEAF = true;
      cursor->size = (this->MAX + 1) / 2;
      newLeaf->size = this->MAX + 1 - (this->MAX + 1) / 2;
      cursor->ptr[cursor->size] = newLeaf;
      newLeaf->ptr[newLeaf->size] = cursor->ptr[this->MAX];
      cursor->ptr[this->MAX] = NULL;
      for (i = 0; i < cursor->size; i++) {
        cursor->key[i] = virtualNode[i];
        cursor->pagePtr[i] = virtualPageNode[i];
        cursor->rowPtr[i] = virtualRowNode[i];
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
        newLeaf->key[i] = virtualNode[j];
        newLeaf->pagePtr[i] = virtualPageNode[j];
        newLeaf->rowPtr[i] = virtualRowNode[j];
      }
      if (cursor == root) {
        Node *newRoot = new Node;
        newRoot->key[0] = newLeaf->key[0];
        newRoot->pagePtr[0] = newLeaf->pagePtr[0];
        newRoot->rowPtr[0] = newLeaf->rowPtr[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      } else {
        insertInternal(newLeaf->key[0], newLeaf->pagePtr[0], newLeaf->rowPtr[0], parent, newLeaf);
      }
    }
  }
}

// Insert Operation
void bplusTree::insertInternal(int x, int pagePtr, int rowPtr, Node *cursor, Node *child) {
  if (cursor->size < this->MAX) {
    int i = 0;
    while (x > cursor->key[i] && i < cursor->size)
      i++;
    for (int j = cursor->size; j > i; j--) {
      cursor->key[j] = cursor->key[j - 1];
      cursor->pagePtr[j] = cursor->pagePtr[j - 1];
      cursor->rowPtr[j] = cursor->rowPtr[j - 1];
    }
    for (int j = cursor->size + 1; j > i + 1; j--) {
      cursor->ptr[j] = cursor->ptr[j - 1];
    }
    cursor->key[i] = x;
    cursor->pagePtr[i] = pagePtr;
    cursor->rowPtr[i] = rowPtr;
    cursor->size++;
    cursor->ptr[i + 1] = child;
  } else {
    Node *newInternal = new Node;
    int virtualKey[this->MAX + 1];
    int virtualPageKey[this->MAX + 1];
    int virtualRowKey[this->MAX + 1];
    Node *virtualPtr[this->MAX + 2];
    for (int i = 0; i < this->MAX; i++) {
      virtualKey[i] = cursor->key[i];
      virtualPageKey[i] = cursor->pagePtr[i];
      virtualRowKey[i] = cursor->rowPtr[i];
    }
    for (int i = 0; i < this->MAX + 1; i++) {
      virtualPtr[i] = cursor->ptr[i];
    }
    int i = 0, j;
    while (x > virtualKey[i] && i < this->MAX)
      i++;
    for (int j = this->MAX + 1; j > i; j--) {
      virtualKey[j] = virtualKey[j - 1];
      virtualPageKey[j] = virtualPageKey[j - 1];
      virtualRowKey[j] = virtualRowKey[j - 1];
    }
    virtualKey[i] = x;
    virtualPageKey[i] = pagePtr;
    virtualRowKey[i] = rowPtr;
    for (int j = this->MAX + 2; j > i + 1; j--) {
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;
    newInternal->IS_LEAF = false;
    cursor->size = (this->MAX + 1) / 2;
    newInternal->size = this->MAX - (this->MAX + 1) / 2;
    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
      newInternal->key[i] = virtualKey[j];
      newInternal->pagePtr[i] = virtualPageKey[j];
      newInternal->rowPtr[i] = virtualRowKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
      newInternal->ptr[i] = virtualPtr[j];
    }
    if (cursor == root) {
      Node *newRoot = new Node;
      newRoot->key[0] = cursor->key[cursor->size];
      newRoot->pagePtr[0] = cursor->pagePtr[cursor->size];
      newRoot->rowPtr[0] = cursor->rowPtr[cursor->size];
      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    } else {
      insertInternal(cursor->key[cursor->size], cursor->pagePtr[cursor->size], cursor->rowPtr[cursor->size]  ,findParent(root, cursor), newInternal);
    }
  }
}

// Find the parent
Node *bplusTree::findParent(Node *cursor, Node *child) {
  Node *parent;
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
    return NULL;
  }
  for (int i = 0; i < cursor->size + 1; i++) {
    if (cursor->ptr[i] == child) {
      parent = cursor;
      return parent;
    } else {
      parent = findParent(cursor->ptr[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}

// Print the tree
void bplusTree::display(Node *cursor) {
    cout << "ANNA" << endl;
  if (cursor != NULL) {
    for (int i = 0; i < cursor->size; i++) {
      cout << cursor->key[i] << " ";
    }
    cout << "\n";
    if (cursor->IS_LEAF == true) {
        cout << "I AM LEAF" << endl;
    }
    else{
        cout << "I AM NOT A LEAF" << endl;
    }
    if (cursor->IS_LEAF != true) {
      for (int i = 0; i < cursor->size + 1; i++) {
        display(cursor->ptr[i]);
      }
    }
  }
}

// Get the root
Node *bplusTree::getRoot() {
  return root;
}
