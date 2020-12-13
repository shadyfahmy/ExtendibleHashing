#include"directory.h"
#include<iostream>

using namespace std;

int main(void)
{

    directory dir;


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

        switch (key)
        {
        case 1:
            dir.search(key);
            break;
                
        case 2:
            dir.insert(key,val);
            break;

        case 3:
            dir.remove(key);
            break;    

        default:
            break;
        }
    }


    return 0;
}