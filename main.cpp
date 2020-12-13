#include "readfile.h"
#include<iostream>

using namespace std;

int main(void)
{

    int directoryFileHandler = createFile(DIRECTORY_FILESIZE,"Directory");
    int bucketsFileHandler = createFile(BUCKETS_FILESIZE,"Buckets");

    while(1){
        int operation;
        int key;
        int val;

        cout << "Choose operation:" << endl;
        cout << "1 -> Search" << endl;
        cout << "2 -> Insert" << endl;
        cout << "3 -> Delete" << endl;
        cin >> operation;
        cout << "Enter a key" << endl;
        cin >> key;
        if(operation == 2)
        {
            cout<<"Enter a value" << endl;
            cin >> val;
        }
        Record r;
        r.key = key;
        r.valid = 1;
        r.value = val;
        int count = 0;
        switch (key)
        {
        case 1:
            searchItem(directoryFileHandler , bucketsFileHandler, r, &count);
            break;
                
        case 2:
            insertItem(directoryFileHandler , bucketsFileHandler, r);
            break;

        case 3:
            deleteItem(directoryFileHandler , bucketsFileHandler, r, &count);
            break;    

        default:
            break;
        }
        // Display directory and buckets
        display(directoryFileHandler, bucketsFileHandler);
    }


    return 0;
}