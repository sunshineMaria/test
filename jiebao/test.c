#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

int main(int argc,char *argv[])
{
    char path[1000];
    // char file[1000];
    /*
    if(argc!=2)
  {
      printf("Usage mk<pathname>\n");
    return 1;
  }
*/
argv[1] = "hoho";
getwd(path);  //取得当前工作目录
printf("current dirctory is:%s\n",path);
if(mkdir(argv[1],S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH)<0)//创建新目录
{
  printf("mkdir failed\n");
  return 2;
}

if(chdir("hoho")<0)  //改变当前工作目录为新目录
{
  printf("chdir failed \n");
  return 3;
}

getwd(path);
printf("mkdir successed.\n New current directory is:%s\n",path);
//rmdir(path);  //删除新建目录
printf("%s is removed\n",path);
return 0;

}