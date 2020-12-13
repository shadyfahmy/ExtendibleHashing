#ifndef READFILE_H_
#define READFILE_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Bucket.h"
#include "Directory.h"


#define MAX_BITS_IN_DIRECTORY 5
#define NUMBER_OF_BUCKETS  (int)pow(2,MAX_BITS_IN_DIRECTORY)    //Maximum Number of BUCKETS
#define BUCKETSIZE sizeof(Bucket)		                        //Size of the bucket (in bytes)
#define BUCKETS_FILESIZE BUCKETSIZE*NUMBER_OF_BUCKETS         //Size of the file 


#define DIRECTORY_FILESIZE sizeof(Directory)                 //Size of directory

//Check the create File
int createFile(int size, char *);

//Extendible hashing implementation
int insertItem(int dirFd, int bucketsFd, Record);
int searchItem(int dirFd, int bucketsFd, Record , int *);
int deleteItem(int dirFd, int bucketsFd, Record , int *);
int display(int dirFd, int bucketsFd);


#endif /* READFILE_H_ */
