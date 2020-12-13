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

}