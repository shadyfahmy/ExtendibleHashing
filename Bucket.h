#ifndef BUCKET_H_
#define BUCKET_H_
#include <bits/stdc++.h>


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
    int currentIndex;
    int localDepth;
    Record records[RECORDS_PER_BUCKET];

    Bucket(/* args */);
    void insertRecord(int,int);
    ~Bucket();
};

#endif