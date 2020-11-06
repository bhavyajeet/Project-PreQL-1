#include "global.h"

hashing::hashing()
{
    logger.log("hashing::hashing");
}

// [] -> -> -> -> 
// [] -> -> -> ->
// [] -> -> -> -> -> ->
// [] -> -> -> -> 
// [] -> -> ->
// [] ->


hashing::hashing(string tableName, int b,int rowCount, int indexedColumnNumber ) 
{ 
    this->BUCKET = b; 
    this->tableName = tableName;
    for (int i = 0; i < b; i++)
    {
      bt* buk = (bt*) malloc(sizeof(bt));
      buk->next = NULL;
      buk->prev = NULL;
      buk->data = -1;
      this->bucks.push_back(buk);
    }
    this->pointerBucket = 0;
} 
  
int sizeOfList(bt * iter){
  int count = 0;
  while (iter)
  {
    count++;
    iter = iter->next;
  }
  return count;
}

void hashing::insertItem(int key, int pagePtr, int rowPtr) 
{
    bt* item = (bt*) malloc(sizeof(bt));
    item->data = key;
    item->pagePtr = pagePtr;
    item->rowPtr = rowPtr;
    item->next = NULL;
    // a + a-. b
    // a + b
    // a b 
    // 0 0 0
    // 0 1 1
    // 1 0 1
    // 1 1 1

//  -> [] -> -> -> -> || -> -> -> -> 
//     [] -> -> 
//     [] -> -> -> -> ||
//     [] -> -> -> ->  -> -> ->

    int OVERFLOW_SIZE = 5;

    item->prev = NULL;
    if(index > this->pointerBucket || sizeOfList(bucks[index]) < OVERFLOW_SIZE){
      int index = hashFunction(key);
      if(index < this->pointerBucket){
        index += BUCKET;
      }
      bt* itr = bucks[index];
      while (itr)
      {
        // cout << itr->data;
        if(itr->next == NULL){
          itr->next = item;
          item->prev = itr;
          break;
        }
        itr = itr->next;
      }
    }
    else if(index == this->pointerBucket && sizeOfList(bucks[index]) >= 5){
       // reHash
       this->pointerBucket++;

       if(this->pointerBucket == BUCKET*2 - 1){
        // increase size of BUCKET
        BUCKET *= 2;
       }
        bt* buk = (bt*) malloc(sizeof(bt));
        buk->next = NULL;
        buk->prev = NULL;
        buk->data = -1;
        int count = 0;
        bt* iter = bucks[index];
        index += BUCKET;
        bucks[index] = buk;
        while (iter)
        {
          count = count + 1;
          if(count > OVERFLOW_SIZE){
            // overflow starts
            bt* save = iter;
            while (iter)
            {
              bt* itr = bucks[index];
              while (itr)
              {
                // cout << itr->data;
                if(itr->next == NULL){
                  itr->next = buk;
                  buk->prev = itr;
                  break;
                }
                itr = itr->next;
              }      
              iter = iter->next;
            }
            save->next = NULL;
          }
          iter = iter->next;
        }
       
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
          itr->prev->next = itr->next;
        }
      }
      itr = itr->next;
    }
    int flag = 0;
    for (int i = BUCKET; i < BUCKET + this->pointerBucket; i++)
    {
      if(sizeOfList(bucks[i]) > 1){
        flag = 1;
      }
    }

    if(!flag){
      // need to delete the bucket
      this->pointerBucket = 0;
    }    
} 
  
// function to display hash table 
int hashing::searchElement(int key) { 
  for (int i = 0; i < BUCKET + this->pointerBucket; i++) { 
    bt* itr = bucks[i];
    while (itr)
    {
      if(iter->data == key){
        cout << "BAZINGA !!!"
        int array = [iter->pagePtr,iter->rowPtr];
        return *array;
      }
    }
    cout << endl;
  }
  int barray = [-1,-1];
  return *barray;
}


// function to display hash table 
void hashing::displayHash() { 
  for (int i = 0; i < BUCKET; i++) { 
    bt* itr = bucks[i];
    while (itr)
    {
      cout << itr->data << endl;
      itr = itr->next;
    }
    cout << endl;
  }
} 