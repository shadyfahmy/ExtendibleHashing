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

int searchItem(int dirFd, int bucketsFd, Record item, int *count) {

}
int deleteItem(int dirFd, int bucketsFd, Record item , int *count){

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