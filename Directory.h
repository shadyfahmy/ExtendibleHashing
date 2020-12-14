#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_NUMBER_OF_ELEMENTS  32

class DirElement{
    public:
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
    void Duplicate(int fd);             // This function duplicates directory
    bool EmptyBucket(int bucketsFd);
    void Merge(int bucketsFd);
    void Shrink();                      // This function shrinks directory
    void mergeAndShrink(int bucketsFd);
    ~Directory();
};
