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
    int currentIndex;   //should be incremented after duplicate and decremented after a shrink (how maney indeces we have now in the elements array)
    DirElement elements[MAX_NUMBER_OF_ELEMENTS];
    
    Directory(/* args */);
    void Duplicate(int fd);            // This function duplicates directory
    void Shrink();               // This function shrinks directory
    void mergeAndShrink(int bucketsFd);
    ~Directory();
};

