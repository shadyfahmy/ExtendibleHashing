#include <bits/stdc++.h>

#define MAX_NUMBER_OF_ELEMENTS  32

class DirElement{
    public:
        int key;
        int bucketOffset;
};

class Directory
{
public:
    /* data */
    int globalDepth;
    DirElement elements[MAX_NUMBER_OF_ELEMENTS];
    
    Directory(/* args */);
    void Duplicate(int fd);            // This function duplicates directory
    void Shrink(int fd);               // This function shrinks directory
    ~Directory();
};

