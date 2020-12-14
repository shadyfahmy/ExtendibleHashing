#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
using namespace std;
#include "Bucket.h"

Bucket::Bucket(/* args */)
{
    this->currentIndex = 0;
    for (int i = 0; i < RECORDS_PER_BUCKET; i++)
      this->records[i].valid = 0;
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

void Bucket::adjustRecords(int index) {
  for(int i=index;i<RECORDS_PER_BUCKET-1;i++) {
    this->records[i].valid = this->records[i+1].valid;
    this->records[i].key = this->records[i+1].key;
    this->records[i].value = this->records[i+1].value;
  }
}

void Bucket::deleteRecord(Record item)
{
    for(int i=0; i<RECORDS_PER_BUCKET; i++) {
      if(this->records[i].key == item.key) {
        this->currentIndex--;
        adjustRecords(i);
        this->records[RECORDS_PER_BUCKET-1].valid = 0;
        this->records[RECORDS_PER_BUCKET-1].key = -1;
        this->records[RECORDS_PER_BUCKET-1].value = 0;
        return;
      }
   }

}

Bucket* Bucket::splitBucket(int key, int val, int bucketNumber, int globalDepth, bool* inserted, int bucketsFd, int* offset)
{
  Bucket* b = new Bucket();
  int newNumber = bucketNumber + (2^(globalDepth - localDepth))/2;
  localDepth++;
  b->localDepth = this->localDepth;
  b->bucketNumber = newNumber;

  for(int i = 0; i < RECORDS_PER_BUCKET; i++)
  {
    if((hashCode(this->records[i].key) >> (MAX_BITS_IN_DIRECTORY - globalDepth)) != bucketNumber)
    {
      b->insertRecord(this->records[i].key, this->records[i].value);
      this->deleteRecord(this->records[i]);
    }
  }

  int hash = hashCode(key);
  if(hash >> (MAX_BITS_IN_DIRECTORY - globalDepth) == bucketNumber && this->currentIndex < RECORDS_PER_BUCKET)
  {
    this->insertRecord(key, val);
    *inserted = true;
  }
  else if(hash >> (MAX_BITS_IN_DIRECTORY - globalDepth) == newNumber && b->currentIndex < RECORDS_PER_BUCKET)
  {
    b->insertRecord(key, val);
    *inserted = true;
  }
  else
  {
   *inserted = false; 
  }
  
  int result;
  Bucket bucketData;
  for(int i = 0; i < NUMBER_OF_BUCKETS; i++)
  {
    result = pread(bucketsFd, &bucketData, sizeof(Bucket),i*sizeof(Bucket));
    /*Empty bucket found */
    if(bucketData.localDepth == 0)
    {
      *offset = i*sizeof(Bucket);
      result = pwrite(bucketsFd,&bucketData ,sizeof(Bucket), *offset);
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