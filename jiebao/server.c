#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

#define BUFFERSIZE 1024
#define CMDSIZE 50
#define TEMPSIZE 1024

struct PRAM{
    int count;
    int fd_accept;
    struct sockaddr_in addr;
};

void strformat(char *str)//除去字符串两端的空格
{
    int length = strlen(str);
	int end=length-1;
	int begin = 0;
	int i;
	while (isspace(str[begin]) && begin < length) {
		begin++;
	}
	while (isspace(str[end]) && end > 0) {
		end--;
	}
	if (begin > end) {
		str[0] ='\0';
	} else {
		for (i=begin; i<=end; i++) {
			str[i-begin] = str[i];
		}
		str[end-begin+1] ='\0';
	}
}

void getstr(char *str)//读取规范字符串
{
    int i = 0;
	while (1) {
		str[i] = getchar();
		if (str[i] == '\n') {
			str[i] ='\0';
			break;
		}
		i++;
	}
	strformat(str);
}

void getcmd(char *cmd,char *str)//获取指令字符串
{
    int i;
    for(i=0; i<=strlen(str); i++)
    {
        if(isspace(str[i])||(str[i]=='\0'))
        {
            cmd[i]='\0';
            break;
        }
        cmd[i]=str[i];
    }
}

void strafter(char *str,char flag)//截取指定字符后的字符串
{
    int len = strlen(str);
    int j = -1;
    int i;
    for(i=0;i<len;i++)
    {
        if(str[i] == flag)
        {
            j = i;
            break;
        }
    }
    if(j >= 0)
    {
        for(i=0;i<len-j-1;i++)
        {
            str[i] = str[i+(j+1)];
        }
        str[len-j-1]='\0';
    }
    strformat(str);
}

void *thr(void *arg){
    pthread_detach(pthread_self());
    char buffer[BUFFERSIZE];
    bzero(buffer,sizeof(buffer));
    char cmd[CMDSIZE];
    char temp[TEMPSIZE];
    struct PRAM *tpram;
    struct PRAM *pram;
    pram=(struct PRAM *)arg;
    tpram=(struct PRAM *)malloc(sizeof(struct PRAM));
    memcpy(tpram,pram,sizeof(struct PRAM));
    int fd_accept=tpram->fd_accept;
    printf("检测到来自 %s 的连接请求\n",inet_ntoa(tpram->addr.sin_addr));
    strcpy(buffer,"成功连接至服务器\n");
    if(write(fd_accept,buffer,sizeof(buffer))<0){
        printf("错误！响应发送失败:连接\n");
        close(fd_accept);
        printf("◆  当前连接数：%d\n",--pram->count);
        free(tpram);
        pthread_exit(NULL);
    }

    if(read(fd_accept,buffer,BUFFERSIZE)<0){
        printf("错误！ 信息接收失败：账户密码\n");
        close(fd_accept);
        printf("◆  当前连接数：%d\n",--pram->count);
        free(tpram);
        pthread_exit(NULL);
    }

    if(strcmp(buffer,"123123")==0){
        printf("登录验证成功，允许进行操作\n");
        strcpy(buffer,"登录成功\n");
        if(write(fd_accept,buffer,sizeof(buffer))<0){
            printf("错误！响应发送失败：成功登录\n");
            close(fd_accept);
            printf("◆  当前连接数：%d\n",--pram->count);
            free(tpram);
            pthread_exit(NULL);
        }
    }
    else{
        strcpy(buffer,"错误");
        if(write(fd_accept,buffer,sizeof(buffer))<0){
            printf("错误！响应发送失败：错误登录\n");
            close(fd_accept);
            printf("◆  当前连接数：%d\n",--pram->count);
            free(tpram);
            pthread_exit(NULL);
        }
        close(fd_accept);
        printf("◆  当前连接数：%d\n",--pram->count);
        free(tpram);
        pthread_exit(NULL);
    }
    while(1){
        if(read(fd_accept,buffer,BUFFERSIZE)<0){
            printf("错误！ 信息接收失败：命令\n");
            close(fd_accept);
            printf("◆  当前连接数：%d\n",--pram->count);
            free(tpram);
            pthread_exit(NULL);
        }
        getcmd(cmd,buffer);
        printf("%s执行的操作：%s\n",inet_ntoa(tpram->addr.sin_addr),buffer);
        if(strcmp(cmd,"mkdir")==0){
            strafter(buffer,' ');
            if(mkdir(buffer,0777)<0){  //最大权限
                printf("错误！命令执行失败：mkdir\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                free(tpram);
                pthread_exit(NULL);
            }
            strcpy(buffer,"mkdir执行成功!\n");
            if( write(fd_accept,buffer,sizeof(buffer))<0){
                printf("错误！响应发送失败：mkdir\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                free(tpram);
                pthread_exit(NULL);
            }
        }else if(strcmp(cmd,"rmdir")==0){
            strafter(buffer,' ');
            if(rmdir(buffer)<0){
                printf("错误！响应发送失败：rmdir\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                free(tpram);
                pthread_exit(NULL);
            }
            strcpy(buffer,"rmdir 执行成功!\n");
            if(write(fd_accept,buffer,sizeof(buffer))<0){
                printf("错误！响应发送失败：rmdir\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                free(tpram);
                pthread_exit(NULL);
            }
        }else if(strcmp(cmd,"cd")==0){
            strafter(buffer,' ');
            if(chdir(buffer)<0){
                printf("没有打开文件夹\n");
                printf("buffer is%s\n",buffer);
                printf("错误！响应发送失败：cd\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                // free(tpram);
                // pthread_exit(NULL);
            }
            printf("chdir正确");
            strcpy(buffer,"cd 执行成功!\n");
            if(write(fd_accept,buffer,sizeof(buffer))<0){
                printf("错误！响应发送失败：cd\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                free(tpram);
                pthread_exit(NULL);
            }
        }else if(strcmp(cmd,"pwd")==0){
            strafter(buffer,' ');
            if(getcwd(NULL,0)<0){
                printf("错误！响应发送失败：cd\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                free(tpram);
                pthread_exit(NULL);
            }
            strcpy(buffer,getcwd(NULL,0));
            printf("%s\n",getcwd(NULL,0));
            if(write(fd_accept,buffer,sizeof(buffer))<0){
                printf("错误！响应发送失败：cd\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                free(tpram);
                pthread_exit(NULL);
            }
        }else if(strcmp(cmd,"ls")==0){
            strafter(buffer,' ');
            DIR *dir;
            struct dirent *ent;
            bzero(temp,sizeof(temp));
            if((dir = opendir(".")) == NULL){
                printf("错误！打开目录文件失败\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                free(tpram);
                pthread_exit(NULL);
            }
            while((ent = readdir(dir))!= NULL){
                printf("读到了");
                struct stat st;
                if(ent == 0)
                    break;
                char *filename = ent->d_name;
                if(stat(filename,&st) == -1){//获取filename信息，保存在st中
                    printf("错误！stat失败\n");
                    closedir(dir);
                    close(fd_accept);
                    printf("◆  当前连接数：%d\n",--pram->count);
                    free(tpram);
                    pthread_exit(NULL);
                }
                else{
                    char mode[11];
                    strcpy(mode,"----------");
                    if(S_ISDIR(st.st_mode))
                    mode[0]='d';
                    if (S_ISCHR(st.st_mode)) 
                    mode[0] = 'c';    /* 字符设备   */
                    if (S_ISBLK(st.st_mode))
                    mode[0] = 'b';    /* 块设备     */
                    if(st.st_mode & S_IRUSR)
                    mode[1]='r';
                    if(st.st_mode & S_IWUSR)
                    mode[2]='w';
                    if(st.st_mode & S_IXUSR)
                    mode[3]='x';
                    if(st.st_mode & S_IRGRP)
                    mode[4]='r';
                    if(st.st_mode & S_IWGRP)
                    mode[5]='w';
                    if(st.st_mode & S_IXGRP)
                    mode[6]='x';
                    if(st.st_mode & S_IROTH)
                    mode[7]='r';
                    if(st.st_mode & S_IWOTH)
                    mode[8]='w';
                    if(st.st_mode & S_IXOTH)
                    mode[9]='x';
                    printf("%s ",mode);
                    printf("%s\n" , filename);
                    strcpy(buffer,filename);
                    printf("目前buffer是：%s\n",buffer);
                    if(write(fd_accept,buffer,sizeof(buffer))<0){
                    printf("错误！响应发送失败\n");
                    close(fd_accept);
                    printf("◆  当前连接数：%d\n",--pram->count);
                    free(tpram);
                    pthread_exit(NULL);
                }

                    
                }

            }
                
            //     
            closedir(dir);
        }else if(strcmp(cmd,"put")==0){
	        int fp;
            int result;
            strafter(buffer,' ');
            if((fp = open(buffer,O_RDWR|O_CREAT|O_APPEND))<0){
                printf("错误！open失败\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                free(tpram);
                pthread_exit(NULL);
            }
            result = read(fd_accept,buffer,sizeof(buffer));
            write(fp,buffer,result);
            close(fp);
            printf("接收%dB的文件\n",result);
        }else if(strcmp(cmd,"get")==0){
            strafter(buffer,' ');
            FILE *file;
            bzero(temp,sizeof(temp));
            if((file = fopen(buffer,"r"))==NULL){
                printf("错误！fopen失败\n");
                close(fd_accept);
                printf("◆  当前连接数：%d\n",--pram->count);
                free(tpram);
                pthread_exit(NULL);
            }
            int num=0;
            num = fread(temp,1,sizeof(temp),file);
            write(fd_accept,temp,num);
            fclose(file);
        }else if(strcmp(cmd,"exit")==0){
            close(fd_accept);
            printf("◆  当前连接数：%d\n",--pram->count);
            break;
        }else{
            close(fd_accept);
            printf("◆  当前连接数：%d\n",--pram->count);
            break;
        }
    }
    free(tpram);
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;
    struct PRAM *arg;
    arg=(struct PRAM *)malloc(sizeof(struct PRAM));
    arg->count=0;
    printf("\n");
   printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("******************欢迎进入FTP服务端*****************\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	int fd_socket=socket(AF_INET,SOCK_STREAM,0);
	if(fd_socket<0){
	    printf("错误！socket初始化失败\n");
	    return -1;
	}
	struct sockaddr_in addr_server;
	struct sockaddr_in addr_client;
	bzero(&addr_server,sizeof(struct sockaddr_in));
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(1234);
	addr_server.sin_addr.s_addr = INADDR_ANY;
    if(bind(fd_socket, (struct sockaddr *)&addr_server, sizeof(addr_server))<0){
         printf("错误！bind失败\n");
	    return -1;
    };
    if(listen(fd_socket,SOMAXCONN)<0){
        printf("错误！listen失败\n");
	    return -1;
    }
    while(1)
    {
        bzero(&addr_client,sizeof(struct sockaddr_in));
        int len = sizeof(addr_client);
        int fd_accept=accept(fd_socket,(struct sockaddr *)&addr_client, &len);
        if(fd_accept<0){
            printf("错误！accept失败\n");
            continue;
        }
        arg->fd_accept=fd_accept;
        memcpy((void*)&arg->addr,&addr_client,sizeof(addr_client));
        printf("◆  当前连接数：%d\n",++arg->count);
        if(pthread_create(&tid,NULL,thr,(void *)arg)){
            printf("错误！线程创建失败\n");
            exit(0);
        }

    }
    close(fd_socket);
    return 0;
}
