#include "Directory.h"
#include "Bucket.h"

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

bool Directory::EmptyBucket(int bucketsFd) {
    for(int i=0;i<currentIndex;i++){
        Bucket data;
        pread(bucketsFd,&data,sizeof(Bucket), elements[i].bucketOffset);
        if(data.currentIndex == 0) {
            return true;
        }
    }
    return false;
}

// void Directory::Merge(int bucketsFd){
//     /*   this while loop made for more than one elements points to the same bucket that has been deleted*/
//     int shiftValue = 1;
//     for(int i=0;i<currentIndex;i++) {
//         Bucket data;
//         pread(bucketsFd,&data,sizeof(Bucket), elements[i].bucketOffset);
//         if(data.currentIndex == 0) {
//             int before = i-shiftValue;
//             int after = i+shiftValue;
            
//             if(((before >> shiftValue) == (i >> shiftValue))) {
//                 Bucket dataBefore;
//                 pread(bucketsFd,&dataBefore,sizeof(Bucket), elements[before].bucketOffset);
//                 if(dataBefore.currentIndex > 0) {
//                     int startIndex = (i >> (shiftValue-1))<<(shiftValue-1);     //this line makes sure we will start at right index of all of those elements points to the same buckets(ex case of 4)
//                     int endIndex = startIndex + pow(2,shiftValue-1);
//                     /*   this loop if there is more than one element points to the same empty bucket all will points to the split image   */
//                     for(int j=startIndex;j<endIndex;j++) {
//                         elements[j].bucketOffset = elements[before].bucketOffset;
//                     }
//                     dataBefore.localDepth--;
//                     pwrite(bucketsFd,&dataBefore,sizeof(Bucket), elements[before].bucketOffset);
//                     break;
//                 } else {
//                     shiftValue++;
//                 }
//             } else if ((after >> shiftValue) == (i >> shiftValue)) {
//                 Bucket dataAfter;
//                 pread(bucketsFd,&dataAfter,sizeof(Bucket), elements[after].bucketOffset);
//                 if(dataAfter.currentIndex > 0) {
//                     int startIndex = (i >> (shiftValue-1))<<(shiftValue-1);     //this line makes sure we will start at right index of all of those elements points to the same buckets(ex case of 4)
//                     int endIndex = startIndex + pow(2,shiftValue-1);
//                     /*   this loop if there is more than one element points to the same empty bucket all will points to the split image   */
//                     for(int j=startIndex;j<endIndex;j++) {
//                         elements[j].bucketOffset = elements[after].bucketOffset;
//                     }
//                     dataAfter.localDepth--;
//                     pwrite(bucketsFd,&dataAfter,sizeof(Bucket), elements[after].bucketOffset);
//                     break;
//                 } else {
//                     shiftValue++;
//                 }
//             }
//         }
//     }
// }

void Directory::Merge(int bucketsFd) {
    for(int i=0;i<currentIndex-1;i+=2) {
        Bucket data1;
        pread(bucketsFd,&data1,sizeof(Bucket), elements[i].bucketOffset);

        Bucket data2;
        pread(bucketsFd,&data2,sizeof(Bucket), elements[i+1].bucketOffset);
        if(data1.currentIndex == 0) {
            elements[i].bucketOffset = elements[i+1].bucketOffset;
        } else if (data2.currentIndex == 0) {
            elements[i+1].bucketOffset = elements[i].bucketOffset;
        }
    }
}

void Directory::Shrink(){
    for(int i=0,j=0;i<currentIndex;i++,j+=2){
        elements[i].bucketOffset = elements[j].bucketOffset;
    }
    currentIndex /= 2;
    globalDepth--;
}

void Directory::mergeAndShrink(int bucketsFd){

    Merge(bucketsFd);

    // bool flage = false;
    // for(int i=0; i<currentIndex; i++) {
    //     Bucket data;
    //     pread(bucketsFd,&data,sizeof(Bucket), elements[i].bucketOffset);
    //     /*   this condition means there is buckets with localdepth same as global and not empty so there is no need for shrink*/
    //     if(globalDepth == data.localDepth && data.currentIndex>0) {
    //         flage = true;
    //         break;
    //     }
    // }

    bool canShrink = true;
    for(int i=0; i<currentIndex-1; i+=2) {
        /*   this condition means there is buckets with localdepth same as global and not empty so there is no need for shrink*/
        if(elements[i].bucketOffset != elements[i+1].bucketOffset) {
            canShrink = false;
            break;
        }
    }

    if(canShrink){
        Shrink();
    }
}

Directory::~Directory()
{
}
