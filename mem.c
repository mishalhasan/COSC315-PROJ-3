#include <stdio.h>
#include <stdlib.h>

  
int main() 
{ 
    FILE *fp; 
    int count = 0;  // Line counter (result) 
    char c;  // To store a character read from file 
    int m; 
    int n; 
  

    // Get file name from user. The file should be 
    // either in current folder or complete path should be provided 
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
            printf("test %d \n",i); 
            i++; 
        }else if(i == 0 )
        {
            n = atoi(line); // offset 
            printf("%d \n", n); 
            i++;   
        }else if (i==1)
        {
            m= atoi(line); //page number, 16-n = m
            printf("%d \n", m); 
            i++; 
        }

    }

    
 
    // Close the file 
    fclose(fp); 
    printf("The file has %d lines\n ",count); 
  
    return 0; }


