#include <string>
#include <iostream>
#include <fstream>
#include "inode.h"
#include "myFileSystem.h"
#include "global.h"

using namespace std;

int main(int argc, char* argv[]){
    //if not the right number of arguments, print how to use and exit
    if (argc == 1 ){
        fprintf(stderr,"usage: %s <inputFileName> \n",argv[0]);
        return(0);
    }
    cout << "Spinning up disk..." << endl;

    ifstream input(argv[1]);
    //if the file didn't open, print message and exit
    if (!input.is_open()){
       cout << argv[1] << " : file not found " << endl;
       return(0);
    }

    //read disk name from input file
    string diskName;
    input >> diskName;

    //initalize new myFileSystem object
    myFileSystem files(diskName);   //segmentation fault here

    string cmd;
    string filename;
    int blockNum;
    static char buf[1024];

    while(!input.eof()){
        //get first section from line
        input >> cmd;

        //if section is L, run ls command
        if(cmd.compare("L") == 0){
           files.ls();
           continue;
        }

        //get the filename from the inputfile
        input >> filename;
        char fileChar[NAME_LENGTH];
        //take first 8 characters into char[] to conform to the available space in the inode
        filename.copy(fileChar, NAME_LENGTH);
       
        //if section is D, run delete
        if(cmd.compare("D") == 0){
           files.del(fileChar);
           continue;
        }

        input >> blockNum;
        //if section is C, run create
        if(cmd.compare("C") == 0){
            files.create(fileChar, blockNum);
        }
        //if section is W, run write
        else if(cmd.compare("W") == 0){
           files.write(fileChar, blockNum, buf);
        }
        //if section is R, run read
        else if(cmd.compare("R") == 0){
           files.read(fileChar, blockNum, buf);
        }
    }
    input.close();
    return(1);
}