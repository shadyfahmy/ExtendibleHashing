#include"bucket.h"
#include <bits/stdc++.h>

class directory
{
private:
    /* data */
    int globalDepth;
    std::vector<bucket*> data;

public:
    directory(/* args */);
    void insert(int, int);
    void remove(int);
    int search(int);
    ~directory();
};

directory::directory(/* args */)
{
    globalDepth = 0;
    bucket initialBucket;
}

void directory::insert(int key, int val)
{

}

void directory::remove(int key)
{

}

int directory::search(int key)
{
    
}


directory::~directory()
{
}
