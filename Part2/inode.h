#ifndef INODE_H
#define INODE_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

typedef int int32; // 4-byte int
typedef char byte; // alias char as byte


class inode{

    public:
        char name[8];           //filename 
        int32 size;             //file size in blocks
        int32 blockPointer[8];  //direct block pointers
        int32 used;             //0 => inode is free; 1 => in use  

        inode();
        inode(char NAME[8], int SIZE, int BLOCKPOINTER[8], int USED);
    
};

#endif
