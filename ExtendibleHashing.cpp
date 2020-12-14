#include "readfile.h"

/* Hash function to choose bucket
 * Input: key used to calculate the hash
 * Output: HashValue;
 */
int hashCode(int key){
   return key % NUMBER_OF_BUCKETS;
}

int insertItem(int dirFd, int bucketsFd,Record item)
{

   Directory data;
   Bucket bucketData;
   ssize_t result = pread(dirFd,&data,sizeof(Directory), 0);

   int hashKey = hashCode(item.key);
   int targetKey = hashKey >> (MAX_BITS_IN_DIRECTORY - data.globalDepth);
   int targetOffset = data.elements[targetKey].bucketOffset;

   /*No Directory Exists*/
   if(data.globalDepth == 0)
   {

   }
   /*Directory Exists*/
   else
   {
      /*Read target bucket*/
      result = pread(bucketsFd,&bucketData,sizeof(Bucket), targetOffset);
      
      /*Target bucket has free space*/
      if(bucketData.currentIndex < RECORDS_PER_BUCKET)
      {
         bucketData.insertRecord(item.key, item.value);
         result = pwrite(bucketsFd,&bucketData ,sizeof(Bucket), targetOffset);
      }
      /*Target bucket is full*/
      else
      {
         bool inserted = false;

         while(!inserted)
         {
            targetKey = hashKey >> (MAX_BITS_IN_DIRECTORY - data.globalDepth);
                
            /*If local depth less than global depth split the bucket*/
            if(bucketData.localDepth < data.globalDepth)
            {
               /*
               Bucket* newBucket = bucketData->splitBucket(key, val, globalDepth, &inserted);

               for(int i = 0; i < 2^(globalDepth - newBucket->getLocalDepth()); i++)
               {
                  data[newBucket->getNumber() + i] = newBucket;
               }
               */
            }
            /*Dublicate the directory*/
            else
            {
               /*
               data.resize(2*data.size());
               for(int i = 0; i < data.size()/2; i++) 
               {
                  data[data.size() - 1 - 2*i] = data[data.size()/2- 1 - i] ;
                  data[data.size() - 1 - 2*i - 1] = data[data.size()/2- 1 - i] ;
               }
               globalDepth++;
               int i = 0;
               while (i < data.size())
               {
                   data[i]->setNumber(i);
                   if(data[i]->getLocalDepth() == globalDepth)
                       i++;
                   else
                       i = i + 2^(globalDepth - data[i]->getLocalDepth());
               }
               */

            }
         }
      }
      
      
   }
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