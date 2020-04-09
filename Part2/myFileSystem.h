#ifndef MYFILESYSTEM_H
#define MYFILESYSTEM_H

#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include "inode.h"

//typedef int int32; // 4-byte int
//typedef char byte; // alias char as byte

using namespace std;

class myFileSystem{

    private:
        inode nodes[NUM_NODES];
        string DiskName;

        /* free-block list */
        char block_list[128]; //if the i-th byte is 0, it indicates that the block is free, else it is in use
        int32 getFreeNode();
        int32 checkNames(char name[NAME_LENGTH]);
        int32* getFreeBlocks(int32 size);
        void writeSuperBlock();
        void writeBlock(int blockNum, char *buf);
        void readBlock(int blockNum, char *buf);

    public:
        myFileSystem(string diskName);
        int32 create(char name[NAME_LENGTH], int32 size);
        int32 del(char name[NAME_LENGTH]);
        void ls();
        int32 read(char name[NAME_LENGTH], int32 blockNum, char buf[1024]);
        int32 write(char name[NAME_LENGTH], int32 blockNum, char buf[1024]);
        void close();


};

#endif