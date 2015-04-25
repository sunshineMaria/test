#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define SIZE 1000
 
         int main(int argc,char *argv[])
         {
               int fd,size;
               char buf[1024];
               if(argc<2)
               {
                      printf("Usage:%s Filename\n",argv[0]);
                      exit(1);
               }
               fd=open(argv[1],O_RDONLY);
               if(fd<0)
               {
                      printf("Fail to open file\n");
                      exit(2);
                 }
                else
                 {
                       size=read(fd,buf,SIZE);
                       if(size<0)
                        {
                                printf("Fail to read file\n");
                                exit(3);
                        }
                       else
                       {
                                printf("From File %s Read %d bytes\n",argv[1],size);
                                buf[size]='\0';
                                printf("%s\n",buf);
                        }
                       close(fd);
                   }
                   return 0;
         }
