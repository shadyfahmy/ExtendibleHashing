#include "readfile.h"
#include<iostream>

#include <iostream>
#include <math.h>
void Test(int, int);

using namespace std;

int main(void)
{

    int directoryFileHandler = createFile(DIRECTORY_FILESIZE,"Directory");
    int bucketsFileHandler = createFile(BUCKETS_FILESIZE,"Buckets");
    int result;
    bool run=true;

    //Test(directoryFileHandler,  bucketsFileHandler);
    display(directoryFileHandler, bucketsFileHandler);

    while(run){
        int operation;
        int key;
        int val;

        cout << "Choose operation:" << endl;
        cout << "1 -> Search" << endl;
        cout << "2 -> Insert" << endl;
        cout << "3 -> Delete" << endl;
        cout << "4 -> Display" << endl;
        cout << "5 -> Exit" << endl;
        cin >> operation;
        
        if(operation != 4 && operation != 5) {
            cout << "Enter a key" << endl;
            cin >> key;
            if(operation == 2)
            {
                cout<<"Enter a value" << endl;
                cin >> val;
            }
        }
        Record r;
        r.key = key;
        r.valid = 1;
        r.value = val;
        int count = 0;
        switch (operation)
        {
        case 1:
            result = searchItem(directoryFileHandler , bucketsFileHandler, r, &count);
            if(result != -1)
                cout << "Record Found at offset = " << result << ", Number of records accessed is " << count << endl;
            else
                cout << "Item Not found, number of records accessed is " << count << endl;
            break;
                
        case 2:
            insertItem(directoryFileHandler , bucketsFileHandler, r);
            break;

        case 3:
            deleteItem(directoryFileHandler , bucketsFileHandler, r, &count);
            break;    
        case 4:
            // Display directory and buckets
            result = display(directoryFileHandler, bucketsFileHandler);
            cout << "Number of records accessed is " << result << endl;
            break; 

        case 5:
            run = false;
            // Display directory and buckets
            result = display(directoryFileHandler, bucketsFileHandler);
            cout << "Number of records accessed is " << result << endl;
            close(directoryFileHandler);
            close(bucketsFileHandler);
            remove("Directory");
            remove("Buckets");
            break; 

        default:
            break;
        }
        
    }


    return 0;
}


void Test(int directoryFileHandler, int bucketsFileHandler){ 
    Directory dir;
    dir.globalDepth = 2;
    dir.elements[0].bucketOffset = 0;

    dir.elements[1].bucketOffset = 40;
    
    dir.elements[2].bucketOffset = 80;

    dir.elements[3].bucketOffset = 120;
    int result = pwrite(directoryFileHandler,&dir,sizeof(Directory), 0);

    Bucket b;
    b.localDepth = 2;
    b.records[0].valid = 1;
    b.records[0].key=32;
    b.records[0].value=15;
    
    b.records[1].valid = 1;
    b.records[1].key=64;
    b.records[1].value=444;
    result = pwrite(bucketsFileHandler,&b ,sizeof(Bucket), 0);
}