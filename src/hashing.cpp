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
      vector <bt*> v;
      v.push_back(buk);
      this->bucks.push_back(v);
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
//     // a + a-. b
//     // a + b
//     // a b 
//     // 0 0 0
//     // 0 1 1
//     // 1 0 1
//     // 1 1 1

// //  -> [] -> -> -> -> || -> -> -> -> 
// //     [] -> -> 
// //     [] -> -> -> -> ||
// //     [] -> -> -> ->  -> -> ->

    int OVERFLOW_SIZE = 2;
    int index = hashFunction(key);
    item->prev = NULL;
    int siz = bucks[index].size();
    // cout << "ANNAYA" << endl;
    // cout << key << endl;
    // cout << index << endl;
    // cout << this->pointerBucket << endl;;
    if(index < this->pointerBucket){
      // these buckets are split, use the new hash function
      index = hashFunction2(key);
      siz = bucks[index].size();
      // cout << "TATATTATTI " << endl;
      if(index < this->pointerBucket && bucks[index].size() == OVERFLOW_SIZE){
        // cout << "ANNNAYA AAA" << endl;
        bucks[index + BUCKET].push_back(item);
      }
    }
    else{
        bucks[index].push_back(item);
        siz = bucks[index].size();
    }

    

    if(siz > OVERFLOW_SIZE){
      // break the pointerBucket
      bt* buk = (bt*) malloc(sizeof(bt));
      buk->next = NULL;
      buk->prev = NULL;
      buk->data = -1;
      vector <bt*> v;
      v.push_back(buk);
      this->bucks.push_back(v);
      // iterate from overflow to end and move this to end 
      for (int i = OVERFLOW_SIZE; i < this->bucks[this->pointerBucket].size(); i++)
      {
        this->bucks[this->pointerBucket + BUCKET].push_back(this->bucks[this->pointerBucket][i]);
      }
      cout << this->bucks[this->pointerBucket].size() << endl;
      if(this->bucks[this->pointerBucket].size() > OVERFLOW_SIZE){
        this->bucks[this->pointerBucket].resize(OVERFLOW_SIZE);
      }
      this->pointerBucket++;

      if(this->pointerBucket == BUCKET ){
        // last bucket has been broken
        BUCKET *= 2;
      }
    }
} 
  
bool hashing::deleteItem(int key) 
{
  // // get the hash index of key 
  // int index = hashFunction(key); 
  // if(index < this->pointerBucket){
  //   index = hashFunction2(key);
  // }
  // int found = 0;
  // int where = 0;
  // for (int i = 1; i < this->bucks[index]; i++)
  // {
  //   if(this->bucks[index][i] == key){
  //     // delete this item
  //     found = 1;
  //     where = i;
  //   }
  // }
  // if(found){
  //   // found it, delete at the position
  //   this->bucks[index].erase(this->bucks[index].begin() + where);
  //   return true;
  // }
  // else{
  //   // not found, kya hi kar sakte hai
  //   return false;
  // }
} 
  
// function to display hash table 
bt* hashing::searchElement(int key) {
  int index = hashFunction(key);
  if(index < this->pointerBucket){
    index = hashFunction2(this->pointerBucket);
  }
  for (int i = 1; i < this->bucks[index].size(); i++) { 
    if(this->bucks[index][i]->data == key){
      return this->bucks[index][i];
    }
  }
  bt* nod = (bt*) malloc(sizeof(bt));
  nod->prev = NULL;
  nod->next = NULL;
  nod->data = -1;
  return nod;
}

// function to display hash table 
void hashing::displayHash() { 
  for (int i = 0; i < this->bucks.size(); i++) { 
    for (int j = 0; j < this->bucks[i].size(); j++)
    {
      cout << "|" << this->bucks[i][j]->data << "|" ;
    }
    cout << endl;
  }
} 