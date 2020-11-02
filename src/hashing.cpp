#include "global.h"

hashing::hashing()
{
    logger.log("hashing::hashing");
}

hashing::hashing(int b) 
{ 
    this->BUCKET = b; 
    // table = new list<int>[BUCKET]; 
} 
  
void hashing::insertItem(int key) 
{ 
    int index = hashFunction(key); 
    bucks[index].push_back(key);  
} 
  
void hashing::deleteItem(int key) 
{ 
  // get the hash index of key 
  int index = hashFunction(key); 
  
  // find the key in (inex)th list 
  list <int> :: iterator i; 
  for (i = bucks[index].begin(); 
           i != bucks[index].end(); i++) { 
    if (*i == key) 
      break; 
  } 
  
  // if key is found in hash table, remove it 
  if (i != bucks[index].end()) 
    bucks[index].erase(i); 
} 
  
// function to display hash table 
void hashing::displayHash() { 
  for (int i = 0; i < BUCKET; i++) { 
    cout << i; 
    for (auto x : bucks[i]) 
      cout << " --> " << x; 
    cout << endl; 
  } 
} 