# COSC315-PROJ-3

## Contributors

* **Spencer Ke** - 44343168 

   - Part 2 
  
 
* **Mishal Hasan** - 20647160 

    - Part 1 & Part 2 
    
*As we are only two people in our group, we split the work 50/50 as much as possible. One person did part 1 and the other did part 2. We worked on the logic together and helped each other with debugging.  *

## Code Design 

### Part 1: Virtual Address Calculation 

The main method opens and reads the file. Whilst reading the file until the end is reached, a counter is incremented for each new line to figure out the total number of lines in a file. The cursor is brought back up to the top of the file. Then a while loop is used to figure out each virtual address page number and offset. Since the first two numbers in the list are not a virtual address, they are stored as regular integers. Any numbers after the first two are used in the methods below.

  * int offset(int k, int value){...) // calculate offset 
     - A mask is created to extract the number of bits the offset is which is represented by n. 
     - The offset is then the mask AND value(virtual address) computed. Computing it this way provides only the last k bits of        the number.  
     - An integer is returned as the value representing the offset. 

  * int pageNum(int value, int k){...) //calculate page number 
    -  Since we know which bit the offset is startting at, we can shift the numbers to the right to remove those bits in order        to figure out the value of the bits representing the page number. 
    
  ### Part 2: Java Build & Executing Instructions 
  
  * Open the Part1 folder on command line 
  * Use gcc to compile all the mem.c file in the Part1 folder 
      ``` gcc mem.c -o test ````
  * Execute the exec files 
      ``` ./test ```
      - The program will automatically print statements regarding the page number and offset of each virtual address 

## Online Resources 

https://stackoverflow.com/questions/8011700/how-do-i-extract-specific-n-bits-of-a-32-bit-unsigned-integer-in-c
