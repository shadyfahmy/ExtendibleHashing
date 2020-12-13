#include <bits/stdc++.h>

class bucket
{
private:
    /* data */
    int size;
    int full;
    int localDepth;
    std::map<int, int> records;


public:
    bucket(/* args */);
    void insertRecord(int,int);
    ~bucket();
};

bucket::bucket(/* args */)
{
    size = 2;   //zai el section
    full = 0;
}

void bucket::insertRecord(int key, int val)
{
    if(full < size)
        records.insert({key, val});
    full++;
}

bucket::~bucket()
{
}
