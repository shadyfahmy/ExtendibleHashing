
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
        for(int i=0; i<this->currentIndex; i++) {
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
    for(int i=0; i<this->currentIndex; i++) {
      if(this->records[i].key == item.key) {
        this->records[i].valid = 0;
        this->records[i].key = -1;
        this->records[i].value = 0;
        this->currentIndex--;
        return;
      }
   }
}

Bucket::~Bucket()
{
}