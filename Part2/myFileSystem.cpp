#include "myFileSystem.h"
#include "global.h"

using namespace std;


//private functions

int myFileSystem::getFreeNode(){
    //find a free inode

    //default is returned if no free node is found
    int freeNode = -1;    

    //exit loop early if free node is found
    //loop also exits at the end of the node list
    for(int i = 0; i < NUM_NODES && freeNode ==  -1; i++){
        if(nodes[i].used == 0){
            freeNode = i;
        }
    }
    
    return freeNode;
}
int myFileSystem::checkNames(char name[NAME_LENGTH]){
    string nameString = name;
    //return -1 if name is free
    //return index of node if there is a match
    int nameIsFree = -1;

    //exit loop early if a match is found
    for(int i = 0; i < NUM_NODES && nameIsFree == 0; i++){
        string nodeName(nodes[i].name);

        if((nameString.compare(nodeName)) == 0 ){
            nameIsFree = i;
        }
    }

    return nameIsFree;

}
int32* myFileSystem::getFreeBlocks(int32 size){
    int32* freeBlocks[NAME_LENGTH];

    //initalize all elements to -1
    for(int i = 0; i < size; i++)
        *freeBlocks[i] = -1;

    //loop through all blocks after superblock and exit early if all the needed blocks have been aquired
    int numFree = 0;  //counter for free blocks found
    for(int i = 1; i < NUM_BLOCKS && *freeBlocks[size-1] == -1; i++){
        if(block_list[i] == '0'){
            *freeBlocks[numFree] = i;
            numFree++;
        }
    }

    return *freeBlocks;
}
void myFileSystem::writeSuperBlock(){
    ofstream diskWriter;
    diskWriter.open(DiskName);
    //seek beginning of file
    diskWriter.seekp(0, diskWriter.beg);
    //write block_list
    diskWriter.write(block_list, 128);
    //write inodes
    for(int i = 0; i < NUM_NODES; i++){
        //store pointer to inode
        inode * temp = &nodes[i];
        //convert to char pointer
        char * tempChar = reinterpret_cast<char*>(temp);
        //write to file from pointer as char
        diskWriter.write(tempChar, sizeof(inode));
    }
    diskWriter.close();
    return;
}
void myFileSystem::readBlock(int blockNum, char *buf){
    ifstream diskReader;
    diskReader.open(DiskName);
    //seek the indicated block to read
    diskReader.seekg(blockNum*BLOCK_SIZE, diskReader.beg);
    //read indicated block into buf
    diskReader.read(buf, BLOCK_SIZE);
    diskReader.close();
    return;
}
void myFileSystem::writeBlock(int blockNum, char *buf){
    ofstream diskWriter;
    diskWriter.open(DiskName);
    //seek the indicated block to write
    diskWriter.seekp(blockNum*BLOCK_SIZE, diskWriter.beg);
    //write into indicated block
    diskWriter.write(buf, BLOCK_SIZE);
    diskWriter.close();
    return;
}









//public functions

myFileSystem::myFileSystem(string disk_name){
   // Open the file with name diskName

   //superblock order is:

   //block list
   //inodes
   cout << "Loading disk..." << endl;
    DiskName = disk_name;

   ifstream diskReader;

    diskReader.open(DiskName);
    //get the superblock
    diskReader.read(block_list, 128);   //get the free block list
    cout << block_list[0] << endl;
    for(int i = 0; i < NUM_NODES; i++){         //get the inodes
        //store pointer in char pointer then convert to inode pointer
        char * tempChar;
        diskReader.read(tempChar, sizeof(inode));
        inode * tempInode = reinterpret_cast<inode*>(tempChar);
        nodes[i] = *tempInode;
    } 
    cout << "Disk loaded!" << endl;
    diskReader.close();

}
int myFileSystem::create(char name[NAME_LENGTH], int32 size){
    
    int freeNode = getFreeNode();
    //if getFreeNode could not find a free node (ie returned a negative value)
    if(freeNode < 0){
        cout << "Disk is full! cannot create " << name  << endl;
        return -1;
    }
    //if checkNames found a node with the same name (ie returned a non-negative index)
    if(checkNames(name) >= 0){
        cout << "A file named "<< name << " already exists."  << endl;
        return -1;
    }
    //check if there is enough blocks free and get the indexes
    int * blocks = getFreeBlocks(size);
    //return error if there weren't enough free blocks to fill the array (last element in array is still the default value)
    if(blocks[size-1] == -1){
        cout << "Not enough space in disk to create "  << name << " with size " << to_string(size) << endl;
        return -1;
    }

    //mark blocks and inode as used and update the information in the inodes
    for(int i = 0; i < size; i++){
        block_list[blocks[i]] = '1';
        nodes[freeNode].blockPointer[i] = blocks[i];
    }
    nodes[freeNode].used = 1;
    nodes[freeNode].size = size;
    for(int i = 0; i < NAME_LENGTH; i++){
        nodes[freeNode].name[i] = name[i];
    }

    //find the start of the file and write the superblock back into the first 1 kb
    writeSuperBlock();
    return 0;
}
int myFileSystem::del(char name[NAME_LENGTH]){
    //find an inode with the same name as passed in
    //return an error if you do not find one with the same name
    int node = checkNames(name);
    //if checkNames did not find a node with the same name (ie returned a negative)
    if(node < 0){
        cout << "There is no file with name " << name << endl;
        return -1;
    }
    
    //update block_list to mark blocks as free
    for(int i = 0; i < NAME_LENGTH; i++){
        block_list[nodes[node].blockPointer[i]] = '0';
    }

    //mark inode as free
    nodes[node].used = 0;

    //write superblock back to file
    writeSuperBlock();
    return 0;
}
void myFileSystem::ls(){

    //print name and size fields on all inodes
    for(int i = 0; i < NUM_NODES; i++){
        printf("%s, %d blocks", nodes[i].name, nodes[i].size);
    }
    return;
}
int myFileSystem::read(char name[NAME_LENGTH], int32 blockNum, char buf[BLOCK_SIZE]){
    //locate inode by the name, same as delete()
    int node = checkNames(name);
    //if checkNames did not find a node with the same name (ie returned a negative)
    if(node < 0){
        cout << "There is no file with name " << name << endl;
        return -1;
    }
    //seek blockPointers[blockNum] of inode and read the block from disk to buf
    readBlock(nodes[node].blockPointer[blockNum], buf);
    return 0;
}
int myFileSystem::write(char name[NAME_LENGTH], int32 blockNum, char buf[BLOCK_SIZE]){
    //locate inode by the name, same as delete()
    int node = checkNames(name);
    //if checkNames did not find a node with the same name (ie returned a negative)
    if(node < 0){
        cout << "There is no file with name " << name << endl;
        return -1;
    }

    //seek blockPointers[blockNum] of inode and write buf to disk
    writeBlock(nodes[node].blockPointer[blockNum], buf);
    return 0;
}
void myFileSystem::close(){
    //provide interface between public and private functions
    writeSuperBlock();
}
