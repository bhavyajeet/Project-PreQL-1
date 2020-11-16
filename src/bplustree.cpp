#include "global.h"

int MAAX ;

Node::Node() {
  key = new int[MAAX + 1];
  pagePtr = new int[MAAX + 1];
  rowPtr = new int[MAAX + 1];
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
	return {-1,-1};
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
        // cout << "Found\n";
        return make_pair(cursor->pagePtr[i],cursor->rowPtr[i]);
      }
    }
    // cout << "Not found\n";
	return {-1,-1};
  }
}

void bplusTree::searchAndUpdate(int x, int pagePtr, int rowPtr) {
  if (root == NULL) {
    cout << "Tree is empty\n";
	return {-1,-1};
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
        // cout << "Found\n";
		if(cursor->pagePtr[i] == pagePtr && cursor->rowPtr[i] == rowPtr + 1){
			// this one has moved backward in the page
			cursor->rowPtr[i]--;
		}
        return make_pair(cursor->pagePtr[i],cursor->rowPtr[i]);
      }
    }
    cout << "Not found\n";
	return {-1,-1};
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
	  // upper bound
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
      cout << cursor->key[i] << " " << "{{" << cursor->pagePtr[i] << "}}((" <<  cursor->rowPtr[i] << "))" << endl;
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

// void bplusTree::display(Node* cursor) {
//     /*
// 		Depth First Display
//     */

//     /*
//         Level Order Display
//     */
//     if (cursor == NULL) return;
//     queue <Node*> q;
//     q.push(cursor);

//     while (!q.empty()) {
//         int sz = q.size();
//         for (int i = 0; i < sz; i++) {
//             Node* u = q.front(); q.pop();

//             //printing keys in self
//             for (int val = 0; val < u->size; val++)
//                 cout << u->key[val] << " ";

//             cout << "|| ";//to seperate next adjacent nodes
            
//             if (u->IS_LEAF != true) {
// 				for (int j = 0; j < u->size; j++)
// 				{
// 					q.push(u->ptr[j]);
// 				}
//                 // for (Node* v : u->ptr2TreeOrData.ptr2Tree) {
//                 //     q.push(v);
//                 // }
//             }
//         }
//         cout << endl;
//     }
// }



// Get the root
Node *bplusTree::getRoot() {
  return root;
}

void bplusTree::removeKey(int x) {
	Node* root = getRoot();

	// If tree is empty
	if (root == NULL) {
		cout << "B+ Tree is Empty" << endl;
		return;
	}

	Node* cursor = root;
	Node* parent;
	int leftSibling, rightSibling;

	// Going to the Leaf Node, Which may contain the *key*
	// TO-DO : Use Binary Search to find the val
	while (cursor->IS_LEAF != true) {
		for (int i = 0; i < cursor->size; i++) {
			parent = cursor;
			leftSibling = i - 1;//left side of the parent node
			rightSibling = i + 1;// right side of the parent node

			if (x < cursor->key[i]) {
				cursor = cursor->ptr[i];
				break;
			}
			if (i == cursor->size - 1) {
				leftSibling = i;
				rightSibling = i + 2;// CHECK here , might need to make it negative
				cursor = cursor->ptr[i+1];
				break;
			}
		}
	}

	// Check if the value exists in this leaf node
	int pos = 0;
	bool found = false;
	for (pos = 0; pos < cursor->size; pos++) {
		if (cursor->key[pos] == x) {
			found = true;
			break;
		}
	}

	int itr = 0;
	// lower bound
	while (x >= cursor->key[itr] && itr < cursor->size)
        itr++;

	// auto itr = lower_bound(cursor->keys.begin(), cursor->keys.end(), x);

	if (itr == cursor->size) {
		cout << "Key Not Found in the Tree" << endl;
		return;
	}
	
	// Shifting the keys and dataPtr for the leaf Node
	for (int i = pos; i < cursor->size - 1; i++) {
		cursor->key[i] = cursor->key[i+1];
	}

	int prev_size = cursor->size;
	cursor->size = prev_size - 1;
	// cursor->ptr2TreeOrData.dataPtr.resize(prev_size - 1);

	// If it is leaf as well as the root node
	if (cursor == root) {
		if (cursor->size == 0) {
			// Tree becomes empty
			root = NULL;
			cout << "Ohh!! Our Tree is Empty Now :(" << endl;
		}
	}
	
	cout << "Deleted " << x << " From Leaf Node successfully" << endl;
	if (cursor->size >= (this->MAX + 1) / 2) {
		//Sufficient Node available for invariant to hold
		return;
	}

	cout << "UnderFlow in the leaf Node Happended" << endl;
	cout << "Starting Redistribution..." << endl;

	//1. Try to borrow a key from leftSibling
	if (leftSibling >= 0) {
		Node* leftNode = parent->ptr[leftSibling];

		//Check if LeftSibling has extra Key to transfer
		if (leftNode->size >= (this->MAX + 1) / 2 + 1) {

			//Transfer the maximum key from the left Sibling
			int maxIdx = leftNode->size - 1;

			// insert at start
			cursor->key[0] = leftNode->key[maxIdx];
			for (int i = 1; i < cursor->size ; i++)
			{
				cursor->key[i] = cursor->key[i - 1];
			}

			cursor->ptr[0] = leftNode->ptr[maxIdx];
			for (int i = 1; i < cursor->size ; i++)
			{
				cursor->ptr[i] = cursor->ptr[i - 1];
			}

			// cursor->keys.insert(cursor->keys.begin(), leftNode->keys[maxIdx]);
			// cursor->ptr2TreeOrData.dataPtr
				// .insert(cursor->ptr2TreeOrData.dataPtr.begin(), leftNode->ptr2TreeOrData.dataPtr[maxIdx]);

			//resize the left Sibling Node After Tranfer
			leftNode->size = maxIdx;
			// leftNode->keys.resize(maxIdx);
			// leftNode->ptr2TreeOrData.dataPtr.resize(maxIdx);

			//Update Parent
			parent->key[leftSibling] = cursor->key[0];
			printf("Transferred from left sibling of leaf node\n");
			return;
		}
	}

	//2. Try to borrow a key from rightSibling
	if (rightSibling < parent->size) {
		Node* rightNode = parent->ptr[rightSibling];

		//Check if RightSibling has extra Key to transfer
		if (rightNode->size >= (this->MAX + 1) / 2 + 1) {

			//Transfer the minimum key from the right Sibling
			//Transfer the maximum key from the left Sibling
			int minIdx = 0;

			// insert at end
			for (int i = 0; i < cursor->size - 1; i++)
			{
				cursor->key[i] = cursor->key[i + 1];
			}
			cursor->key[cursor->size - 1] = rightNode->key[minIdx];

			for (int i = 0; i < cursor->size - 1; i++)
			{
				cursor->ptr[i] = cursor->ptr[i + 1];
			}
			cursor->ptr[cursor->size - 1] = rightNode->ptr[minIdx];


			// cursor->keys.push_back(rightNode->keys[minIdx]);
			// cursor->ptr2TreeOrData.dataPtr
			// 	.push_back(rightNode->ptr2TreeOrData.dataPtr[minIdx]);

			//resize the right Sibling Node After Tranfer
			// rightNode->keys.erase(rightNode->keys.begin());
			// rightNode->ptr2TreeOrData.dataPtr.erase(rightNode->ptr2TreeOrData.dataPtr.begin());

			//Update Parent
			parent->key[rightSibling-1] = rightNode->key[0];
			printf("Transferred from right sibling of leaf node\n");
			return;
		}
	}

	// Merge and Delete Node
	if (leftSibling >= 0) {// If left sibling exists
		Node* leftNode = parent->ptr[leftSibling];
		//Transfer Key and dataPtr to leftSibling and connect ptr2next
		for (int i = 0; i < cursor->size; i++) {
			leftNode->key[leftNode->size] = cursor->key[i];
			leftNode->ptr[leftNode->size++] = cursor->ptr[i];
			// leftNode->keys.push_back(cursor->keys[i]);
			// leftNode->ptr2TreeOrData.dataPtr
			// 	.push_back(cursor->ptr2TreeOrData.dataPtr[i]);
		}
		// leftNode->ptr2next = cursor->ptr2next;
		cout << "Merging two leaf Nodes" << endl;
		// removeInternal(parent->keys[leftSibling], parent, cursor);//delete parent Node Key
		//delete cursor;
	}
	else if (rightSibling <= parent->size) {
		Node* rightNode = parent->ptr[rightSibling];
		//Transfer Key and dataPtr to rightSibling and connect ptr2next
		for (int i = 0; i < rightNode->size; i++) {
			cursor->key[cursor->size] = rightNode->key[i];
			cursor->ptr[cursor->size++] = rightNode->ptr[i];
			// cursor->keys.push_back(rightNode->keys[i]);
			// cursor->ptr2TreeOrData.dataPtr
			// 	.push_back(rightNode->ptr2TreeOrData.dataPtr[i]);
		}
		// cursor->ptr2next = rightNode->ptr2next;
		cout << "Merging two leaf Nodes" << endl;
		// removeInternal(parent->keys[rightSibling-1], parent, rightNode);//delete parent Node Key
		//delete rightNode;
	}

}


void bplusTree::removeInternal(int x, Node* cursor, Node* child) {
	Node* root = getRoot();

	// Check if key from root is to deleted
	if (cursor == root) {
		if (cursor->size == 1) {
			// If only one key is left and matches with one of the
			// child Pointers
			if (cursor->ptr[1] == child) {
				this->root = cursor->ptr[0];
				//delete cursor;
				cout << "Wow! New Changed Root" <<endl;
				return;
			}
			else if (cursor->ptr[0] == child) {
				this->root = cursor->ptr[1];
				//delete cursor;
				cout << "Wow! New Changed Root" << endl;
				return;
			}
		}
	}

	// Deleting key x from the parent
	int pos;
	for (pos = 0; pos < cursor->size; pos++) {
		if (cursor->key[pos] == x) {
			break;
		}
	}
	for (int i = pos; i < cursor->size-1; i++) {
		cursor->key[i] = cursor->key[i + 1];
	}
	cursor->size = cursor->size - 1;

	// Now deleting the ptr2tree
	for (pos = 0; pos < cursor->size; pos++) {
		if (cursor->ptr[pos] == child) {
			break;
		}
	}

	for (int i = pos; i < cursor->size - 1; i++) {
		cursor->ptr[i] = cursor->ptr[i + 1];
	}

	// If there is No underflow. Phew!!
	if (cursor->size >= (this->MAX + 1) / 2 - 1) {
		cout << "Deleted " << x << " from internal node successfully\n";
		return;
	}

	cout << "UnderFlow in internal Node! What did you do :/" << endl;

	if (cursor == root) {
		return;
	}

	Node* p1 = findParent(root, cursor);
	Node* parent = p1;

	int leftSibling, rightSibling;

	// Finding Left and Right Siblings as we did earlier
	for (pos = 0; pos < parent->size; pos++) {
		if (parent->ptr[pos] == cursor) {
			leftSibling = pos - 1;
			rightSibling = pos + 1;
			break;
		}
	}

	// If possible transfer to leftSibling
	if (leftSibling >= 0) {
		Node* leftNode = parent->ptr[leftSibling];

		//Check if LeftSibling has extra Key to transfer
		if (leftNode->size >= (this->MAX + 1) / 2 ) {

			//transfer key from left sibling through parent
			int maxIdx = leftNode->size - 1;

			// insert at start
			cursor->key[0] = leftNode->key[maxIdx];
			for (int i = 1; i < cursor->size ; i++)
			{
				cursor->key[i] = cursor->key[i - 1];
			}

			cursor->ptr[0] = leftNode->ptr[maxIdx];
			for (int i = 1; i < cursor->size ; i++)
			{
				cursor->ptr[i] = cursor->ptr[i - 1];
			}
			leftNode->size = maxIdx;

			// cursor->keys.insert(cursor->keys.begin(), parent->keys[leftSibling]);
			// parent->keys[leftSibling] = leftNode->keys[maxIdxKey];

			// int maxIdxPtr = leftNode->ptr2TreeOrData.ptr2Tree.size()-1;
			// cursor->ptr2TreeOrData.ptr2Tree
			// 	.insert(cursor->ptr2TreeOrData.ptr2Tree.begin(), leftNode->ptr2TreeOrData.ptr2Tree[maxIdxPtr]);

			// //resize the left Sibling Node After Tranfer
			// leftNode->keys.resize(maxIdxKey);
			// leftNode->ptr2TreeOrData.dataPtr.resize(maxIdxPtr);

			return;
		}
	}

	// If possible transfer to rightSibling
	if (rightSibling < parent->size) {
		Node* rightNode = parent->ptr[rightSibling];

		//Check if LeftSibling has extra Key to transfer
		if (rightNode->size >= (this->MAX + 1) / 2) {

			//transfer key from right sibling through parent
			int maxIdxKey = rightNode->size - 1;

			cursor->key[cursor->size++] = parent->key[pos];
			parent->key[pos] = rightNode->key[0];

			for (int i = 1; i < rightNode->size; i++)
			{
				rightNode->key[i] = rightNode->key[i - 1];
			}
			rightNode->size--;

			// cursor->keys.push_back(parent->keys[pos]);
			// parent->keys[pos] = rightNode->key[0];
			// rightNode->keys.erase(rightNode->keys.begin());

			//transfer the pointer from rightSibling to cursor
			cursor->ptr[cursor->size++] = rightNode->ptr[0];
			// cursor->ptr2TreeOrData.ptr2Tree
			// 	.push_back(rightNode->ptr2TreeOrData.ptr2Tree[0]);
			for (int i = 1; i < cursor->size; i++)
			{
				cursor->key[i] = cursor->key[i - 1];
			}
			cursor->size--;
			// cursor->ptr2TreeOrData.ptr2Tree
			// 	.erase(cursor->ptr2TreeOrData.ptr2Tree.begin());
			 
			return;
		}
	}

	//Start to Merge Now, if None of the above cases applied
	if (leftSibling >= 0) {
		//leftNode + parent key + cursor
		Node* leftNode = parent->ptr[leftSibling];
		leftNode->key[leftNode->size++] = parent->key[leftSibling];
		// leftNode->keys.push_back(parent->keys[leftSibling]);
		int orig = leftNode->size - 1;
		for (int i = 0; i < cursor->size; i++)
		{
			leftNode->key[leftNode->size++] = cursor->key[i];
		}
		
		// for (int val : cursor->keys) {
		// 	leftNode->keys.push_back(val);
		// }

		for (int i = 0; i < cursor->size; i++) {
			leftNode->ptr[i + orig + 1] = cursor->ptr[i];
			// leftNode->ptr2TreeOrData.ptr2Tree
			// 	.push_back(cursor->ptr2TreeOrData.ptr2Tree[i]);
			cursor->ptr[i] = NULL;
		}

		cursor->size = 0;
		// cursor->ptr2TreeOrData.ptr2Tree.resize(0);
		// cursor->keys.resize(0);

		removeInternal(parent->key[leftSibling], parent, cursor);
		cout << "Merged with left sibling"<<endl;
	}
	else if (rightSibling < parent->size) {
		//cursor + parentkey +rightNode
		Node* rightNode = parent->ptr[rightSibling];

		int orig = cursor->size;

		cursor->key[cursor->size++] = parent->key[rightSibling - 1];
		// cursor->keys.push_back(parent->keys[rightSibling - 1]);

		// for (int val : rightNode->keys) {
		// 	cursor->keys.push_back(val);
		// }

		for (int val = 0; val < rightNode->size; val++)
		{
			cursor->key[cursor->size++] = rightNode->key[val];
		}
		

		for (int i = 0; i < rightNode->size; i++) {
			cursor->ptr[i + orig + 1] = rightNode->ptr[i];
			// cursor->ptr2TreeOrData.ptr2Tree
			// 	.push_back(rightNode->ptr2TreeOrData.ptr2Tree[i]);
			rightNode->ptr[i] = NULL;
		}

		// rightNode->ptr2TreeOrData.ptr2Tree.resize(0);
		// rightNode->keys.resize(0);

		rightNode->size = 0;

		removeInternal(parent->key[rightSibling - 1], parent, rightNode);
		cout << "Merged with right sibling\n";
	}
}

