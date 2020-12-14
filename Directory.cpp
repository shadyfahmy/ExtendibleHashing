//#include "Directory.h"
#include "Bucket.h"
#include "readfile.h"

Directory::Directory(/* args */)
{
    globalDepth = 0;
    currentIndex = 0;
}

void Directory::Duplicate(int fd)
{
    currentIndex = 2*currentIndex;
    for(int i = 0; i < currentIndex/2; i++) 
    {
       elements[currentIndex - 1 - 2*i] = elements[currentIndex/2- 1 - i] ;
       elements[currentIndex - 1 - 2*i - 1] = elements[currentIndex/2- 1 - i] ;
    }
    globalDepth++;
    int result = pwrite(fd,this,sizeof(Directory),0);
    /*
    int i = 0;
    while (i < currentIndex)
    {
        data[i]->setNumber(i);
        if(data[i]->getLocalDepth() == globalDepth)
            i++;
        else
            i = i + 2^(globalDepth - data[i]->getLocalDepth());
    }
    */
}

void Directory::Shrink(){

}

void Directory::mergeAndShrink(int bucketsFd){
    bool flage = false;
    for(int i=0; i<currentIndex; i++) {
        Bucket data;
        pread(bucketsFd,&data,sizeof(Bucket), elements[i].bucketOffset);
        /*   this condition means there is buckets with localdepth same as global and not empty so there is no need for shrink*/
        if(globalDepth == data.localDepth && data.currentIndex>0) {
            flage = true;
            break;
        }
    }

    if(flage){
        //Merge();
    } else {
        Shrink();
    }
}

Directory::~Directory()
{
}
