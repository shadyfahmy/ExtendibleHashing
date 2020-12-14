#ifndef BUCKET_H_
#define BUCKET_H_
#include <bits/stdc++.h>
#define MAX_BITS_IN_DIRECTORY 5
#define NUMBER_OF_BUCKETS  (int)pow(2,MAX_BITS_IN_DIRECTORY)    //Maximum Number of BUCKETS



#define RECORDS_PER_BUCKET 2 //Max No. of records inside each Bucket

//Data Record inside the file
class Record {
    public:
        int valid;    //) means invalid record, 1 = valid record
        int value;     
        int key;
};

class Bucket
{
public:
    /* data */
    int currentIndex;   /*this now indicates to how maney elements we have not the last index in the array*/
    int localDepth;
    Record records[RECORDS_PER_BUCKET];

    Bucket(/* args */);
    void insertRecord(int,int);
    int hashCode(int key);
    Bucket* splitBucket(int, int,int*,int, int, bool*);
    void deleteRecord(Record item);
    ~Bucket();
};

#endif