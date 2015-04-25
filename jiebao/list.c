#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
 

void do_ls(char *dirname);

void dostat(char *filename);

void show_file_info(char *filename, struct stat *info_p);

void mode_to_letters(int mode, char * str);

char *uid_to_name(uid_t uid);

char *gid_to_name(gid_t gid);

 

int main(int argc, char **argv)

{

  if (argc == 1)

  { 

    do_ls( "." );

  }

  else

  { 

    while ( --argc )

    {

      printf("%s:\n", *++argv );

      chdir(*argv); //切换到指定目录后再显示该目录的内容

      do_ls( *argv );

      chdir(""); //再回到当前工作目录来

    }

  }

  return 0;

}

 

void do_ls( char * dirname )

{

  DIR    *dir_ptr;

  struct dirent  *direntp;

  /*

  struct dirent //为了获取某文件夹目录内容，所使用的结构体

  {

    ino_t d_nio;

    ff_t d_off;

    signed short int d_reclen;

    unsigned char d_type;

    har d_name[256];  

  }

  d_ino:目录进入点的inode

  d_off:目录文件开头至此目录进入点的位移

  d_reclen:d_name的长度，不包含NULL字符

  d_type:d_ame所指文件类型

  d_name:文件名

  */

  /*

    opendir函数用来打开参数name指定的目录，

    并返回DIR *形态的目录流

  */

  if ( ( dir_ptr = opendir( dirname ) ) == NULL ) 

  {

    fprintf(stderr,"ls1: cannot open %s, not a directory. treat as a file shown below:\n", dirname);

    dostat( dirname ); //如果不是目录就当作文件来显示其属性

  }

  else

  {

    /*

      readdir返回参数dir目录流的下个目录进入点

    */

    while ( ( direntp = readdir( dir_ptr ) ) != NULL )

    { 

      dostat( direntp->d_name );/*逐个显示目录里文件信息*/

    }

    closedir(dir_ptr);

  }

}

 

void dostat( char *filename )

{

  struct stat info;

  if ( stat(filename, &info) == -1 )    /*通过文件名filename获取文件信息，并保存在info所指的结构体stat中*/

  {  

    perror( filename );

  }

  else          /*显示文件信息*/

  { 

    show_file_info( filename, &info );

  }

}

 

void show_file_info(char *filename, struct stat *info_p)

{

  char modestr[11];

 

  mode_to_letters(info_p->st_mode, modestr);//st_mode为文件的类型和存储的权限

 

  printf("%s"  , modestr);

  //st_nlink连接该文件的硬链接数，刚建立的文件值为1

  printf("%4d "  , (int) info_p->st_nlink);  

  printf("%-8s " , uid_to_name(info_p->st_uid));

  printf("%-8s " , gid_to_name(info_p->st_gid));

  printf("%8ld " , (long)info_p->st_size);

  printf("%.12s ", 4+ctime(&info_p->st_mtime));

  printf("%s\n" , filename);

}

 

void mode_to_letters(int mode, char * str)

{

  strcpy(str, "----------");           /* default=无参数 */

 

  if (S_ISDIR(mode))  

    str[0] = 'd';    /* 目录      */

  if (S_ISCHR(mode)) 

    str[0] = 'c';    /* 字符设备   */

  if (S_ISBLK(mode))  

    str[0] = 'b';    /* 块设备     */

 

  if (mode & S_IRUSR) //按位与，看mode是否有S_IRUSR权限

    str[1] = 'r';    /* 用户权限  */

  if (mode & S_IWUSR) 

    str[2] = 'w';

  if (mode & S_IXUSR) 

    str[3] = 'x';

 

  if (mode & S_IRGRP) 

    str[4] = 'r';    /* 组权限 */

  if (mode & S_IWGRP) 

    str[5] = 'w';

  if (mode & S_IXGRP)

    str[6] = 'x';

 

  if (mode & S_IROTH) 

    str[7] = 'r';    /* 其人的权限 */

  if (mode & S_IWOTH) 

    str[8] = 'w';

  if (mode & S_IXOTH) 

    str[9] = 'x';

}

 

char *uid_to_name(uid_t uid)

{

  struct  passwd *pw_ptr;

  static  char numstr[10];

  //获取用户ID

  if ((pw_ptr = getpwuid(uid)) == NULL)

  {

    sprintf(numstr,"%d", uid);

    return numstr;

  }

  else

  { 

    return pw_ptr->pw_name ;

  }

}

 

char *gid_to_name( gid_t gid )

{

  struct group *grp_ptr;

  static  char numstr[10];

  //获取组ID

  if ((grp_ptr = getgrgid(gid)) == NULL)

  {

    sprintf(numstr,"%d", gid);

    return numstr;

  }

  else

  { 

    return grp_ptr->gr_name;

  }

}

