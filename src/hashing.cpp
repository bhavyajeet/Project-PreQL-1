#include "global.h"

hashing::hashing()
{
    logger.log("hashing::hashing");
}

hashing::hashing(int b, string tableName) 
{ 
    this->BUCKET = b; 
    this->tableName = tableName;
    for (int i = 0; i < b; i++)
    {
      bt* buk = (bt*) malloc(sizeof(bt));
      buk->next = NULL;
      buk->prev = NULL;
      buk-> data = -1;
      this->bucks.push_back(buk);
    }
    this->pointerBucket = 0;
} 
  
void hashing::insertItem(int key, int pagePtr, int rowPtr) 
{ 
    int index = hashFunction(key);
    bt* item = (bt*) malloc(sizeof(bt));
    item->data = key;
    item->pagePtr = pagePtr;
    item->rowPtr = rowPtr;
    bt* itr = bucks[index];
    while (itr)
    {
      if(itr->next == NULL){
        itr->next = item;
        item->prev = itr;
      }
      itr = itr->next;
    }
    
} 
  
void hashing::deleteItem(int key) 
{ 
  // get the hash index of key 
  int index = hashFunction(key); 
  int found = 0;
  // find the key in (inex)th list 
    bt* itr = bucks[index];
    while (itr)
    {
      if(itr->data == key){
        if(itr->next == NULL){
          itr->prev->next = NULL;
        }
        else{
          itr->prev->next = iter->next;
        }
      }
      itr = itr->next;
    }
} 
  
// function to display hash table 
void hashing::displayHash() { 
  for (int i = 0; i < BUCKET; i++) { 
    cout << i; 
    for (auto x : bucks[i]) 
      cout << " --> " << x->data; 
    cout << endl; 
  } 
} 