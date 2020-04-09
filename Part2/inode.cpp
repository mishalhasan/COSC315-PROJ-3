#include "inode.h"
#include <algorithm>

using namespace std;

inode::inode(){
    for(int i = 0; i < 8; i++){
        blockPointer[i] = ' ';
        name[i] = ' ';
    }
    size = 0;
    used = 0;
}

inode::inode(char NAME[8], int SIZE, int BLOCKPOINTER[8], int USED){
    for(int i = 0; i < 8; i++){
        blockPointer[i] = BLOCKPOINTER[i];
        name[i] = NAME[i];
    }
               
    size = SIZE;
    used = USED;

}