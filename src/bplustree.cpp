#include "global.h"

bplusTree::bplusTree(){
	
}

bplusTree::bplusTree(string tableName,string thirdParam, int rowCount, int indexedColumnNumber){
	this->fanOut = stoi(thirdParam);
}