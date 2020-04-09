# COSC-315-Proj3

## Contributors

* **Spencer Ke** - 44343168 (50%)

   - C++ Sempahores Version 
  
 
* **Mishal Hasan** - 20647160 (50%)

    - Java Monitors Version 
    
*As we are only two people in our group, we split the work 50/50. One person did part 1 and the other did part 2. We worked on the logic together and helped each other with debugging. The monitors implementation was a lot easier than the semaphores implentation as pthreads was more complicated to use. The monitors implementation did not have as much thinking on the lock complexities, and thus was able to structure the code much easier. *

## Code Design
### Part 1: C++ Filesystem Implementation

The main method reads the input text file, then executes the instructions contained within.

On the invocation of the compiled binary file, the user has to input the name of the input file. If a name is not present, the program will output a message to communicate the correct usage of the binary executable and exit the `main()` function. If a file with a name matching the one entered is not found, the program will commucate the issue to the user by printing a message and exiting the `main()` function.

Having determined that the input file exists and can be opened, the program then takes the disk name that is the first line and creates a new `myFileSystem` object with the retrieved disk name in the `DiskName` attribute. For each line of the file after, the command is parsed and executed appropriately. The valid commands are as follows: 

"L": `ls()`, "D":`del(fileName)`, "C":`create(fileName, blockNum)`, "W":`write(fileName, blockNum, buf)`, "R":`read(fileName, blockNum, buf)`


* `myFileSystem.h` contains the definition of the `myFileSystem` class, which is expanded upon in `myFileSystem.cpp`.

Public functions:
    - `int32 create(char name[8], int32 size)` is the function called to create a new file inside the filesystem. The logic for this function is as follows:
    
    First a free `inode` is identified with the private function `getFreeNode()`. If there is no free node, a message is printed to the user and the function returns a -1 value. Then the private function `checkNames(string name)` is used to check to see if the desired filename is already in use. If it is in use, this is communicated to the user and the function returns -1. Next, the amount of space on the disk is checked with the private function `getFreeBlocks(int size)`. If there are not enough free blocks, this is communicated to the user and the function will return -1. Finally, once it have been determined that the name is unique and there is enough space to create the file, the `inode` and blocks returned by the `getFreeNode()` and `getFreeBlocks(int size)` function are marked as used, and the superblock is written back into the first block of the disk using the `writeSuperBlock()` private function.
    - `int32 del(char name[8])` is the function called to delete a file from the filesystem. The logic for this function is as follows:

    First, the function checks to see if there is a file with a name matching the name passed in using the private `checkNames(string name)` function. If not, this is communicated to the user and the function returns -1. Then, the `inode` and blocks belonging to the located file are marked as free and the superblock is written back into the disk using the `writeSuperBlock()` private function.
    - `void ls()` is the function called to list the files inside the file system. This is done by iterating through the array of `inode`s and printing out the name of the file stored in the `inode`.
    - `int32 read(char name[8], int32 blockNum, char buf[1024])` is the function called to read from a file. The logic for this function is as follows:

    First, locate the file by the name passed into the function in the same manner as in `del()`. Then the block passed in is used to read from the disk to `buf` using the private function `readBlock(int blockNum, char *buf)`.
    - `int32 write(char name[8], int32 blockNum, char buf[1024])` is the function used to write to a file. The logic for this function is as follows:

    First, locate the file by the name passed into the function in the same manner as in `del()`. Then the block passed in is used to read from the disk to `buf` using the private function `writeBlock(int blockNum, char *buf)`.
    - `void close()` is the function called to interface the `writeSuperBlock()` private function with the user without allowing them direct access.


    Private functions:
    - `int32 getFreeNode()` is the function used to determine if there are any free `inodes`, and returns the index of the first free `inode` if there is one. Otherwise, it returns -1. To do this, the `inode` array is iterated through and if a free `inode` is found, the index of that `inode` is returned from the function. If a free inode is not found, the function returns -1.
    -  `int32 checkNames(char name[8])` is the function used to check if there is a file with the supplied name in the filesystem already. To do this, the `inode` is iterated through and each `name` attribute is compared to the provided name. if a match is found, the function returns the index of the `inode` that contains the files. If not, the function returns -1.
    -  `int32* getFreeBlocks(int32 size)` is the function used to determine if there are enough free block to create a file of the desired size. the logic behind this function is as follows:

    First, an array of `int` with size 8 is created. Each element is initalized to -1. Then, the `block_list` is tereated through, with the indices of free blocks stored in the previously created array. The loop exits once the created array is filled with `size` number of indices. If there are not enough free blocks, the `size-1`th element will stay the inital value of -1 and indicate to the calling program that there were not enough free blocks.
    - `void writeSuperBlock()` is the function used to write the `bloc_list` and `inode` array into the first block of the filesystem. First, the start of the file is sought using the `seekp` function of the `ofstream` object used to write to the file system. Then the block_list is written, and the `inode` array is written last. The `ofstream` object is closed before the function exits.
    - `void readBlock()` is the function used to read an indicated block into the buf. First, the indicated block is sought using the `seekg` function of the `ifstream` object used to read from the file system. Then the block is read into the buf and the `ifstream` object is closed before the function exits.
    - `void writeBlock()` is the function used to read an indicated block into the buf. First, the indicated block is sought using the `seekp` function of the `ofstream` object used to write to the file system. Then the block is written in from the buf and the `ifstream` object is closed before the function exits.



### Part 2: C++ Build & Executing Instructions
* Open the C++ file in the terminal.
* Use g++ to compile and link all the `.cpp` and `.h` files: `g++ -pthread -g ${fileDirname}/*.cpp ${fileDirname}/*.h -o ${fileDirname}/315Proj2C++`
    - On my machine, the command looked like this: `g++ -pthread -g /home/spencer/Desktop/315Proj3/Part2/*.cpp /home/spencer/Desktop/315Proj3/Part2/*.h -o /home/spencer/Desktop/315Proj2/315Proj3`
* Execute the binary file `./315Proj3`
    - On the invocation of the compiled binary file, the user has to input the name of the input file, for example `./315Proj3 lab3input.txt`
    - I was unable to run the entirety of the code. There was a segmentation fault when reading the superblock from the fileSystem file. The line with the issue is marked with `//segmentation fault here`
* The values that were used as constants in this program are stored in `global.h`. These inlcude `NAME_LENGTH`(8), `NUM_NODES`(16), `NUM_BLOCKS`(128), and `BLOCK_SIZE`(1024). If one wishes to exampd the capabilities of the `myFileSystem` class, these are the variables to modify.


























The main method initalizes the buffer and the master and slave threads, then runs their processes.
First the user is asked for the number of threads/size of the buffer and the maximum job length. The threads run their singular `run()` method and use the `pull()` and `push()` methods of the `global_buffer`. Once the master and slave threads are started, the `main()` function goes to sleep for a user defined time to allow the threads to run. Afterwards, the `main()` function exits the program, killing all associated threads.

The `pull()` and `push()` methods of the `Buffer` class are designed to be synchronized to allow multiple threads to be working at the same time. To accomplish this, whenever the buffer is being manipulated a binary semaphore is called, denying access to any other thread until the current manipulation is complete.

To prevent the consumer threads from accessing jobs that arent there, the `pull()` method waits until the buffer is not empty. This is done via a binary semaphore that starts locked and unlocks when the `push()` method adds an job to the buffer while there is a thread waiting for the buffer to contain a job.

To prevent the producer thread from overwriting jobs in the buffer, the `push()` method waits until the buffer is not full. This is done via a binary semaphore that starts unlocked and locks if the buffer is full. This semaphore is unlocked when the `pull()` method takes a job from the buffer while the producer thread is waiting for a place in the buffer to place a job.


* `FakeJob.h` contains the defintion of the `FakeJob` class, which is used to encapsulte the jobs created by the `Producer` class and consumed by the `Consumer` class. `Job` is a reserved term in C++, so the name `FakeJob` was used instead.
    - `static int num_jobs` is used to determine the id of the job. The creation of a new instance of a `FakeJob` increments this variable.
    - `private int id` stores the id of the job.
    - `private int length` stores the length of the job.
* `Buffer.h` contains the definition of the `Buffer` class. This definition is expanded upon in `Buffer.cpp`. The class contains:
    - `private` variables to facilitate the functioning of the buffer including `int maxSize`, `int front`, `int rear`, `int bufLen`, and `std::vector<FakeJob> Q`.
    - `sem_t lock, sem_t full, sem_t empty` are the binary semaphores used to control synchronization.
    - `public void push(FakeJob job)` is the method used by the `Producer` class to add jobs to the buffer. The logic for this method is as follows:
    
        If the buffer is full, wait for an element to get pulled out. Lock the `lock` semaphore and add `job` to the rear of the buffer. If there are any threads waiting for a `FakeJob` to be in the buffer, signal using `sem_post` to wake them up. Unlock the `lock` semaphore.
    - `public void pull()` is the method used by the `Consumer` class to take jobs from the buffer. The logic for this method is as follows:
    
        If the buffer is empty, wait for a job to get added. Lock the `lock` semaphore and remove the element at index `front`. If there are any threads waiting for an element to be taken from the buffer, signal using `sem_post` to wake them up. unlock the `lock` semaphore. Return the `FakeJob` instance that was pulled from the front of the buffer.
* `global.h` contains the `global_buffer` instance and is included in all header files except the `Buffer.h` to allow global access to this instance.
* `Producer.h` contains the definition of the `Producer` class. This definition is expanded upon in `Producer.cpp`. The class conatins:
    - `private int maxJobLength` holds the maximum length of a job the producer creates.
    - `public pthread_t thread` holds the thread to run the Producer methods. I would have preferred for this to be a private variable, but it needed to be public to allow pthread_create to access it.
    - `public FakeJob CreateJob()` creates and returns a new instance of `FakeJob` with a length between 1 and the maximum job length allowed
    - `public void wait()` finds a random number for how long the producer thread will go to sleep for, prints a message to indicate the producer is going to sleep for the given amount of time and then calls the `sleep()` function with the appropriate value.
    - `public void run()` holds the logic for what a Producer thread is to do when running. This is as follows:
    
        In a while loop set to `true`, a new job is created by the `CreateJob()` method. The time is recorded and amessage is printed out to indicate the producer created a job with a given length at the time indicated. The new job is pushed into the buffer via the `Buffer::push()` method og `global_buffer`. The `wait()` function is then called. Repeat everything inside the while loop.
* `Consumer.h` contains the definition for the `Consumer` class. The definition is expanded upon in `Consumer.cpp`. The class contains:
    - `private int Tid` holds the id of the consumer thread.
    - `private FakeJob threadJob` holds the job that the `Consumer` trhead is currently working on.
    - `public pthread_t thread` holds the thread to run the Producer methods. I would have preferred for this to be a private variable, but it needed to be public to allow pthread_create to access it.
    - `public void run()` holds the logic for what a Consumer thread is to do while running. This is as follows:
    
        In a while loop set to `true`, a job is pulled out of the buffer via the `Buffer::pull()` method and stored in the `threadJob` attridute. The time is recorded and a message is printed to indicate which thread has taken which job at what time. The job is processed, and then the time is recorded and a message is printed to idicate which thread completed which job at what time. Repeat everything inside the while loop.

### Part 2: C++ Build & Executing Instructions
* Open the C++ file in the terminal.
* Use g++ to compile and link all the `.cpp` and `.h` files: `g++ -pthread -g ${fileDirname}/*.cpp ${fileDirname}/*.h -o ${fileDirname}/315Proj2C++`
    - On my machine, the command looked like this: `g++ -pthread -g /home/spencer/Desktop/315Proj2/*.cpp /home/spencer/Desktop/315Proj2/*.h -o /home/spencer/Desktop/315Proj2/315Proj2C++`
* Execute the binary file `./315Proj2C++`
    - The command line will ask for trhee inputes, type them in. They are assumed to be `int`s.
        - number of threads
        - maximum job size
        - length of time until program exits
    - The program will automatically print messages regarding the workins of the program.
* The producer sleep time is stored in the global `const int sleepT` in the `global.h` file.