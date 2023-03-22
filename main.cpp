#include "mbed.h"
#include "C12832.h"
#include "LM75B.h"

 
//LocalFileSystem local("local");               // Create the local filesystem under the name "local"
 
float temp_v[] = {19.3, 23.8, 26.8, 13.4, 17.9, 19};


int main() {   
    float min_t = 50;
    float max_t = -50;
    for(int i=0; i<10; i++){

        //Min value
        if(temp_v[i] < min_t)
            min_t=temp_v[i];

        //Max value
        if(temp_v[i] > max_t)
            max_t = temp_v[i];
    }


   printf("%f is max and %f is min \r\n", max_t, min_t);
   
    
   
   
   
   
    FILE *fp = fopen("/local/out.txt", "w");  // Open "out.txt" on the local file system for writing
    fprintf(fp, "%f");
    fclose(fp);



}