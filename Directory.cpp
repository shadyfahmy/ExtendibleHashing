//#include "Directory.h"
#include "Bucket.h"
#include "readfile.h"

Directory::Directory(/* args */)
{
    globalDepth = 0;
    currentIndex = 0;
}

void Directory::Duplicate(int fd){

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
