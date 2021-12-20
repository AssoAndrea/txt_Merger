#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS 1


FILE * CreateFile(long size1,long size2,char *name,char *data1,char *data2);
void CloseAllFile(FILE **files);
int RdFile(char *data,long size, FILE *file);




int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        fprintf(stderr,"specify 2 file to merge and new file name");
        return -1;
    }
    const char *file_1 = argv[1];
    const char *file_2 = argv[2];
    const char *newName = argv[3];

    //open 2 file and read size

    FILE *file_1_h =fopen(file_1,"r");
    if (!file_1_h)
    {
        fprintf(stderr,"unable to open file %s",file_1);
    }

    FILE *file_2_h =fopen(file_2,"r");
    if (!file_2_h)
    {
        fprintf(stderr,"unable to open file %s",file_1);
    }

    FILE **files[2];
    files[0] = file_1_h;
    files[1] = file_2_h;
    if (fseek(file_1_h, 0, SEEK_END)) // move the file cursor at the end
    {
        perror("fseek1()");
        CloseAllFile(files);
        return -1;
    }
    long fileSize_1 = ftell(file_1_h);

    if (fseek(file_2_h, 0, SEEK_END))
    {
        perror("fseek2()");
        CloseAllFile(files);
        return -1;
    }
    long fileSize_2 = ftell(file_2_h);
    long finalSize = fileSize_1+fileSize_2;

    //alloco memoria per file 1
    char *data1 = malloc(fileSize_1);
    if (!data1)
    {
        perror("malloc1()");
        CloseAllFile(files);
        return -1;
    }

    if(RdFile(data1,fileSize_1,file_1_h) == -1)
    {
        CloseAllFile(files);
        free(data1);
    }

    //alloco memoria per file 2
    char *data2 = malloc(fileSize_2);

    if (!data2)
    {
        perror("malloc1()");
        CloseAllFile(files);
        return -1;
    }

    if(RdFile(data2,fileSize_2,file_2_h) == -1)
    {
        CloseAllFile(files);
        free(data2);
    }

    FILE *finalFile = CreateFile(fileSize_1,fileSize_2,newName,data1,data2);
    free(data1);
    free(data2);
    CloseAllFile(files);
    fclose(finalFile);
    
    
    
    
    

    
    return 0;
}

int RdFile(char *data,long size, FILE *file)
{
    //metto il puntatore del file all'inizio
    if (fseek(file,0,SEEK_SET))
    {
        perror("fseek_0_1()");
        return -1;
    }

    int ret = fread(data,1,size,file);
    if (ret != size)
    {
        fprintf(stderr,"unable to red file 1");
        return -1;
    }
    return 0;
}

void CloseAllFile(FILE **files)
{
    if(fclose(files[0])!=0)
    {
        perror("fail to close");
    }

    if(fclose(files[1])!=0)
    {
        perror("fail to close");
    }
}

FILE * CreateFile(long size1,long size2,char *name,char *data1,char *data2)
{
    long finalSize = size1+size2;

    FILE *newFile = fopen(name,"w");
    if(!newFile){
        perror("unable to create file");
    }
    fseek(newFile,0,SEEK_SET);
    fwrite(data1,1,size1,newFile);
    fwrite(data2,1,size2,newFile);
    //fclose(newFile);
    return newFile;
}
