#include "readfile.h"

/* Hash function to choose bucket
 * Input: key used to calculate the hash
 * Output: HashValue;
 */
int hashCode(int key){
   return key % NUMBER_OF_BUCKETS;
}

int insertItem(int dirFd, int bucketsFd,Record item){
}

int searchItem(int dirFd, int bucketsFd, Record item, int *count) {
   
   Directory data;
   Bucket bucketData;
   ssize_t result = pread(dirFd,&data,sizeof(Directory), 0);

   // Case global depth is equal 0, Read first bucket 
   if(data.globalDepth == 0) {
      result = pread(dirFd,&bucketData,sizeof(Bucket), 0);
      for(int i = 0; i< RECORDS_PER_BUCKET; i++){
         count++;
         if ((bucketData.records[1].valid == 1) && (bucketData.records[1].key == item.key))
            return 0;                                                    // Offset of the bucket
      }
      // Item not found 
      return -1;
   }

   // Case any depth
   int hashedKey = hashCode(item.key);

   bitset<MAX_BITS_IN_DIRECTORY> x(hashedKey);
   string keyBits = x.to_string().substr(MAX_BITS_IN_DIRECTORY-data.globalDepth,MAX_BITS_IN_DIRECTORY-1);
   

   result = pread(bucketsFd,&bucketData,sizeof(Bucket), data.elements[stoi(keyBits)].bucketOffset);
   if(result < 0)
   { 	  
      perror("some error occurred in pread");
      return -1;
   }

   for(int i = 0; i< RECORDS_PER_BUCKET; i++){
      count++;
      if ((bucketData.records[i].valid == 1) && (bucketData.records[i].key == item.key))
         return data.elements[stoi(keyBits)].bucketOffset; 
   }
   // Not found.
   return -1;

   
   
}
int deleteItem(int dirFd, int bucketsFd, Record item , int *count){

}
int display(int dirFd, int bucketsFd){
   // Displaye Directory
   Directory data;
   Bucket bucketData;
   int count = 0;
   ssize_t result = pread(dirFd,&data,sizeof(Directory), 0);
   if(result < 0)
   { 	  perror("some error occurred in pread");
         return -1;
   } else
   {
      cout << "-------Directory-------" << endl;
      cout << "Global Depth = " << data.globalDepth << endl;
      cout << endl;
      for(int i =0; i< (int)pow(2,data.globalDepth); i++)
      {
         bitset<MAX_BITS_IN_DIRECTORY> x(data.elements[i].key);
         cout << "Element#" << i+1 << "  Key= " << x.to_string().substr(MAX_BITS_IN_DIRECTORY-data.globalDepth,MAX_BITS_IN_DIRECTORY-1)  << ", Offset= " << data.elements[i].bucketOffset << endl;
         ssize_t result = pread(bucketsFd,&bucketData,sizeof(Bucket), data.elements[i].bucketOffset);
         cout << "Bucket Data: Local Depth=" << bucketData.localDepth << endl;
         for(int j=0; j< RECORDS_PER_BUCKET; j++){
            count ++;
            if (bucketData.records[j].valid == 1)
               cout << "Record#" << i+1 << "  Key= " << bucketData.records[j].key << ", Value=" << bucketData.records[j].value << endl;
            else 
               cout << "Record#" << i+1 << " => empty" << endl;
         }
         cout << endl;
      }
      return count;
   }
}