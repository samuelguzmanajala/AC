#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE * fd = fopen("prueba.dat", "r");

    for(int i = 0; i<20;i++){
        for(int j=0;j<5;j++){
            char str = (char) i;
            str = str + (char) j;
            fscanf(fd, "%s",str);
        }
    }
    fclose(fd);
    return 0; 
}