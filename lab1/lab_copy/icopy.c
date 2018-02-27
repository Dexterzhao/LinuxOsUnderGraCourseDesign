#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fp_src, *fp_des;
    char buf[128];
    int num;

    // arg check
    if(argc != 3)
    {
        printf("using like this: icopy file_src file_des\n");
        exit(EXIT_FAILURE);
    }

    // src check
    if((fp_src = fopen(argv[1], "r")) == NULL)
    {
        perror("fopen1");
        exit(EXIT_FAILURE);
    }

    // dst check
    if((fp_des = fopen(argv[2],"w")) == NULL)
    {
        perror("fopen2");
        exit(EXIT_FAILURE);
    }

    // file copy
    do
    {
        num = fread(buf, 1, 128, fp_src);
        fwrite(buf, 1, num, fp_des);
        if(feof(fp_src) == 1)
        {
            break;
        }
    }while(1);
    
    fclose(fp_src);
    fclose(fp_des);
}
