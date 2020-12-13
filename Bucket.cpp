
#include "Bucket.h"

Bucket::Bucket(/* args */)
{
    this->currentIndex = 0;
}

void Bucket::insertRecord(int key, int val)
{
    if(this->currentIndex<RECORDS_PER_BUCKET) {
        this->records[this->currentIndex].key = key;
        this->records[this->currentIndex].value = val;
        this->records[this->currentIndex].valid = 1;
        this->currentIndex++;
    }
}

Bucket::~Bucket()
{
}