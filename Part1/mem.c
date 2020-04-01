#include <stdio.h>
#include <stdlib.h>

  
int main() 
{ 
    FILE *fp; 
    int count = 0;  // Line counter (result) 
    char c;  // To store a character read from file 
    int m; 
    int n; 
  

    // Open file 
    if ((fp = fopen("sample.txt","r")) == NULL){
       printf("Error! opening file");

       // Program exits if the file pointer returns NULL.
       exit(1);
   }
      
      // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n'){ // Increment count if this character is newline 
            count = count + 1;
        }


    rewind(fp); 
    char line[count];
    int i = 0;
    while (fgets(line, sizeof(line), fp)) {
        if(i>1){
            int val = atoi(line); 
            printf("virtual address %d is in page number %d and offset %d \n", val ,pageNum(val,n), offset(n,val));
            i++; 
        }else if(i == 0 )
        {
            n = atoi(line); // offset --> n bits
            i++;   
        }else if (i==1)
        {
            m= atoi(line); //page number, 16-n = m bits 
            i++; 
        }

    }

    
 
    // Close the file 
    fclose(fp);   
    return 0; }

int offset(int k, int value){
    unsigned  mask;
    mask = (1 << k) - 1; //k =n 
    return value & mask; 
}

int pageNum(int value, int k){
    return (value >> k); 
}
