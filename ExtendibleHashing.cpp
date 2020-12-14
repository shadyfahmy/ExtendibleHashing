#include "readfile.h"
#include "Bucket.h"

/* Hash function to choose bucket
 * Input: key used to calculate the hash
 * Output: HashValue;
 */
int hashCode(int key){
   return key % NUMBER_OF_BUCKETS;
}

bool insertItem(int dirFd, int bucketsFd,Record item)
{

   Directory data;
   Bucket bucketData;
   ssize_t result = pread(dirFd,&data,sizeof(Directory), 0);

   int hashKey = hashCode(item.key);

   while(1)
   {
      /*No Directory Exists*/
      if(data.globalDepth == 0)
      {
         result = pread(bucketsFd, &bucketData, sizeof(Bucket),0);

         /*If empty place exists in initial bucket*/
         if(bucketData.currentIndex < RECORDS_PER_BUCKET)
         {
            bucketData.insertRecord(item.key, item.value);
            result = pwrite(bucketsFd,&bucketData ,sizeof(Bucket), 0);
            return true;
         }
         /*No empty place exists in initial bucket*/
         else
         {
            data.globalDepth = 1;
            data.currentIndex = 2;
            bucketData.bucketNumber = 0;
            bucketData.localDepth = 1;
            data.elements[0].bucketOffset = 0;

            Bucket b;
            b.bucketNumber = 1;
            b.localDepth = 1;
            data.elements[1].bucketOffset = sizeof(Bucket);

            int targetKey = hashKey >> (MAX_BITS_IN_DIRECTORY - data.globalDepth);
            bool inserted = false;

            for(int i = 0; i < RECORDS_PER_BUCKET; i++)
            {
              if((hashCode(bucketData.records[i].key) >> (MAX_BITS_IN_DIRECTORY - data.globalDepth)) != 0)
              {
                b.insertRecord(bucketData.records[i].key, bucketData.records[i].value);
                bucketData.deleteRecord(bucketData.records[i]);
              }
            }
            if(targetKey == 0 && bucketData.currentIndex < RECORDS_PER_BUCKET)
            {
              bucketData.insertRecord(item.key, item.value);
              inserted = true;
            }
            else if(targetKey == 1 && b.currentIndex < RECORDS_PER_BUCKET)
            {
              b.insertRecord(item.key, item.value);
              inserted = true;
            }
            else
            {
             inserted = false; 
            }

            if(inserted)
            {
               result = pwrite(bucketsFd,&b ,sizeof(Bucket), sizeof(Bucket));
               result = pwrite(dirFd,&data,sizeof(Directory),0);
               return true;
            }
            else
               continue;
         }

      }
      /*Directory Exists*/
      else
      {
         int targetKey = hashKey >> (MAX_BITS_IN_DIRECTORY - data.globalDepth);
         int targetOffset = data.elements[targetKey].bucketOffset;
         bool inserted = false;

         /*Read target bucket*/
         result = pread(bucketsFd,&bucketData,sizeof(Bucket), targetOffset);

         /*Target bucket has free space*/
         if(bucketData.currentIndex < RECORDS_PER_BUCKET)
         {
            bucketData.insertRecord(item.key, item.value);
            result = pwrite(bucketsFd,&bucketData ,sizeof(Bucket), targetOffset);
            result = pwrite(dirFd,&data,sizeof(Directory),0);
            return true;
         }

         /*Target bucket is full*/
         else
         {
            while(!inserted)
            {
               targetKey = hashKey >> (MAX_BITS_IN_DIRECTORY - data.globalDepth);
               targetOffset = data.elements[targetKey].bucketOffset;
               result = pread(bucketsFd,&bucketData,sizeof(Bucket), targetOffset);
               int offset;

               /*If local depth less than global depth split the bucket*/
               if(bucketData.localDepth < data.globalDepth)
               {
                  Bucket* newBucket = bucketData.splitBucket(item.key, item.value, bucketData.bucketNumber, data.globalDepth, &inserted, bucketsFd, &offset);

                  for(int i = 0; i < 2^(data.globalDepth - newBucket->localDepth); i++)
                  {
                     data.elements[newBucket->bucketNumber + i].bucketOffset = offset;
                  }
                  if (inserted)
                  {
                     result = pwrite(dirFd,&data,sizeof(Directory),0);
                     return true;
                  }
                  continue;

               }
               /*Dublicate the directory*/
               else
               {
                  if(data.globalDepth < MAX_BITS_IN_DIRECTORY)
                  {
                     data.Duplicate(dirFd);
                         int i = 0;
                     while (i < data.currentIndex)
                     {
                        Bucket temp;
                        result = pread(bucketsFd, &temp, sizeof(Bucket),i*sizeof(Bucket));
                        temp.bucketNumber = i;
                        if(temp.localDepth == data.globalDepth)
                        {
                           result = pwrite(bucketsFd,&temp ,sizeof(Bucket), i*sizeof(Bucket));
                           i++;
                        }
                        else
                        {
                           result = pwrite(bucketsFd,&temp ,sizeof(Bucket), i*sizeof(Bucket));
                           i = i + 2^(data.globalDepth - temp.localDepth);
                        }
                     }
                  }
                  /*Can not duplicate the directory --- insertion fails*/
                  else
                     return false;

               }
            }
         }      
      }
   }
}

int searchItem(int dirFd, int bucketsFd, Record item, int *count) {
   
   Directory data;
   Bucket bucketData;
   ssize_t result = pread(dirFd,&data,sizeof(Directory), 0);
   *count = 0;

   // Case global depth is equal 0, Read first bucket 
   if(data.globalDepth == 0) {
      result = pread(bucketsFd,&bucketData,sizeof(Bucket), 0);
      for(int i = 0; i< RECORDS_PER_BUCKET; i++){
         (*count)++;
         if ((bucketData.records[i].valid == 1) && (bucketData.records[i].key == item.key))
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
   if(dir.globalDepth > 0) {
      dir.mergeAndShrink(bucketsFd);
      pwrite(dirFd,&dir,sizeof(Directory), 0);
   }
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
         bitset<MAX_BITS_IN_DIRECTORY> x(i);
         if(data.globalDepth != 0)
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