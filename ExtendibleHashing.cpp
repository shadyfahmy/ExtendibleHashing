#include "readfile.h"

/* Hash function to choose bucket
 * Input: key used to calculate the hash
 * Output: HashValue;
 */
int hashCodeChaining(int key){
   return key % NUMBER_OF_BUCKETS;
}

int insertItem(int dirFd, int bucketsFd,Record item){
}

int searchItem(int dirFd, int bucketsFd, Record item, int *count) { //returns offset

}

int deleteItem(int dirFd, int bucketsFd, Record item , int *count){
   int * diff = 0;

   /*   search for the item we need to delete   */
   int Offset = searchItem(dirFd, bucketsFd, item, diff);

   /*   if not found return -1 indicates to error has happend   */
   if(Offset<0) {
      return -1;
   }

   /*   get the Bucket that has the item, delete the item inside of it and rewrite the data   */
   Bucket data;
   pread(bucketsFd,&data,sizeof(Bucket), Offset);
   data.deleteRecord(item);
   pwrite(bucketsFd,&data,sizeof(Record), Offset);

   /*   if the bucket still not empty nothing needs to be done   */
   if(data.currentIndex > 0) {
      return 0;
   }

   /*   get the directory data for merge and shrink it after deleting a Bucket*/
   Directory dir;
   pread(dirFd,&dir,sizeof(Directory), 0);
   dir.mergeAndShrink(bucketsFd);
}


int display(int dirFd, int bucketsFd){
   // Displaye Directory
   Directory data;
   ssize_t result = pread(dirFd,&data,sizeof(Directory), 0);
   if(result < 0)
   { 	  perror("some error occurred in pread");
         return -1;
   } else
   {
      for(int i =0; i< (int)pow(2,data.globalDepth); i++)
      {
         bitset<MAX_BITS_IN_DIRECTORY> x(data.elements[i].key);
         cout << "Element#" << i+1 << "-Key: " << x.to_string()[0,data.globalDepth] << ", Offset: " << data.elements[i].bucketOffset << endl;
      }
   }
}