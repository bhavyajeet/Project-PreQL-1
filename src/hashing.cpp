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

void hashing::reHash(int key){
  vector < vector< bt* > > temp;
  for (int i = 0; i < this->bucks.size(); i++)
  {
    temp.push_back(this->bucks[i]);
  }
  this->bucks.clear();
  for (int i = 0; i < BUCKET * 2; i++)
  {
    bt* buk = (bt*) malloc(sizeof(bt));
    buk->next = NULL;
    buk->prev = NULL;
    buk->data = -1;
    vector <bt*> v;
    v.push_back(buk);
    this->bucks.push_back(v);
  }
  for (int i = 0; i < temp.size(); i++)
  {
    for (int j = 1; j < temp[i].size(); j++)
    { 
      // aifsn 
      int kkey = temp[i][j]->data % (2 * BUCKET);
      this->bucks[kkey].push_back(temp[i][j]);
    }
  }
  this->pointerBucket = 0;
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
      bucks[index].push_back(item);
      siz = bucks[index].size();
      // cout << "TATATTATTI " << endl;
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
      vector < bt* > a1 ;
      vector < bt* > a2 ;
      a1.push_back(buk);
      a2.push_back(buk);
      
      // k % BUCKET == k % 2*BUCKET;
      for (int i = 1; i < this->bucks[this->pointerBucket].size(); i++)
      {
        int kkey = hashFunction2(this->bucks[this->pointerBucket][i]->data);
        cout << this->bucks[this->pointerBucket][i]->data <<  "][" << hashFunction2(this->bucks[this->pointerBucket][i]->data) <<  endl;
        if(kkey == hashFunction(this->bucks[this->pointerBucket][i]->data)){
          a1.push_back(this->bucks[this->pointerBucket][i]);
        }
        else{
          a2.push_back(this->bucks[this->pointerBucket][i]);
        }
      }
      cout << this->bucks[this->pointerBucket].size() << endl;
      this->bucks[hashFunction(this->pointerBucket)] = a1;
      this->bucks[(this->pointerBucket + BUCKET)] = a2;
      this->pointerBucket++;

      if(this->pointerBucket == BUCKET ){
        // last bucket has been broken
        // reHash everything
        this->reHash(BUCKET);
        BUCKET *= 2;
      }
    }
} 
  
bool hashing::deleteItem(int key, int pagePtr, int rowPtr) 
{
  // [0]8, 16, 24
  // [1]9,17, 25
  // [2]erwetre ,ewr,e esh 
  // [3],ewr sg eh 
  // [4]sfd,g, e s,,g 
  // [5] 3,4,5,
  // [6] 
  // [7] 1


  // get the hash index of key 
  int index = hashFunction(key); 
  if(index < this->pointerBucket){
    index = hashFunction2(key);
  }
  int found = 0;
  int where = 0;
  for (int i = 1; i < this->bucks[index].size(); i++)
  {
    if(this->bucks[index][i]->data == key && this->bucks[index][i]->pagePtr == pagePtr && this->bucks[index][i]->rowPtr == rowPtr){
      // delete this item
      found = 1;
      where = i;
    }
  }
  if(found){
    // found it, delete at the position
    this->bucks[index].erase(this->bucks[index].begin() + where);

    // deleting stuff

    int ind = index;
    if(ind == (this->pointerBucket + BUCKET) ){
      while (this->bucks[ind].size() == 1)
      {
        // this->bucks.erase(this->bucks.start() + ind)
        ind--;
      }
    }

    return true;
  }
  else{
    // not found, kya hi kar sakte hai
    return false;
  }
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
    if(i == this->pointerBucket){
      cout << ">>" ;
    }
    for (int j = 0; j < this->bucks[i].size(); j++)
    {
      cout << "|" << this->bucks[i][j]->data << "|" ;
    }
    cout << endl;
  }
} 