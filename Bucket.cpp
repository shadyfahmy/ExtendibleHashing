
#include "Bucket.h"

Bucket::Bucket(/* args */)
{
    this->currentIndex = 0;
}

void Bucket::insertRecord(int key, int val)
{
    if(this->currentIndex<RECORDS_PER_BUCKET) {
        /*  this for loop to for all availabe places in the array not just at the current index
            this beacuse we may delete an item from the array but not the last one so that would make a place empty and if we kept the condition as is it will say the backet is full    */
        for(int i=0; i < RECORDS_PER_BUCKET; i++) {
          if(this->records[i].valid == 0) {
            this->records[this->currentIndex].key = key;
            this->records[this->currentIndex].value = val;
            this->records[this->currentIndex].valid = 1;
            this->currentIndex++;
            return;
          }
        }
        
    }
}

void Bucket::deleteRecord(Record item)
{
    for(int i=0; i<RECORDS_PER_BUCKET; i++) {
      if(this->records[i].key == item.key) {
        this->records[i].valid = 0;
        this->records[i].key = -1;
        this->records[i].value = 0;
        this->currentIndex--;
        return;
      }
   }
}

Bucket* Bucket::splitBucket(int key, int val, int* nIndex, int index, int globalDepth, bool* inserted)
{
  Bucket* b = new Bucket();
  *nIndex = index + (2^(globalDepth - localDepth))/2;
  localDepth++;
  b->localDepth = this->localDepth;

  for(int i = 0; i < RECORDS_PER_BUCKET; i++)
  {
    if((hashCode(this->records[i].key) >> (MAX_BITS_IN_DIRECTORY - globalDepth)) != index)
    {
      b->insertRecord(this->records[i].key, this->records[i].value);
      this->deleteRecord(this->records[i]);
    }

    int hash = hashCode(key);
    if(hash >> (MAX_BITS_IN_DIRECTORY - globalDepth) == index && this->currentIndex < RECORDS_PER_BUCKET)
    {
      this->insertRecord(key, val);
      *inserted = true;
    }
    else if(hash >> (MAX_BITS_IN_DIRECTORY - globalDepth) == *nIndex && b->currentIndex < RECORDS_PER_BUCKET)
    {
      b->insertRecord(key, val);
      *inserted = true;
    }
    else
    {
     *inserted = false; 
    }
    
  }
  
  return b;

}

int Bucket::hashCode(int key){
   return key % NUMBER_OF_BUCKETS;
}

Bucket::~Bucket()
{
}