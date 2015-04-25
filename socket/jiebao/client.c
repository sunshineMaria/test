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
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#define BUFFERSIZE 1024
#define CMDSIZE 50
#define TEMPSIZE 256

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

int main()
{
    char ip[15];
    char port[6];
    char name[20];
    char pass[10];
    char cat[30];
    char buffer[BUFFERSIZE];
    char cmd[CMDSIZE];
    char hcmd[CMDSIZE];
    char temp[TEMPSIZE];
    printf("\n");
	 printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("******************欢迎进入FTP客户端*****************\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
   printf("                     小组成员：王晓婷   张婕  李永明       \n");
   printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n");
	printf(">>>请连接\n");
	printf("\n");
	printf("**输入IP地址：");
    getstr(ip);
	printf("\n");
	printf("**输入端口号：");
    getstr(port);
	printf("\n");

	int fd_socket=socket(AF_INET,SOCK_STREAM,0);
	if(fd_socket<0){
	    printf("错误！socket初始化失败\n");
	    return -1;
	}
    struct sockaddr_in addr_client;
    bzero(&addr_client,sizeof(struct sockaddr_in));
	addr_client.sin_family=AF_INET;
	addr_client.sin_port=htons(atoi(port));
	if(inet_pton(AF_INET,ip,&addr_client.sin_addr)<0){
	    printf("错误！IP地址转换失败\n");
	    return -1;
	}

	if(connect(fd_socket, (struct sockaddr *)&addr_client,sizeof(struct sockaddr))<0){
	    printf("错误！连接失败\n");
	    return -1;
	}
    if(read(fd_socket,buffer,BUFFERSIZE)<0){
        printf("错误！ 连接信息接收失败\n");
        return -1;
    }
    printf("%s\n",buffer);

	printf(">>>请登录\n");
	printf("\n");
	printf("**输入帐号：");
	getstr(name);
	printf("\n");
    printf("**输入密码：");
	getstr(pass);
	printf("\n");
	strcpy(cat,name);
	strcat(cat,pass);
	if(write(fd_socket,cat,sizeof(cat))<0){
	    printf("错误！帐号密码发送失败\n");
	    return -1;
	}
	if(read(fd_socket,buffer,BUFFERSIZE)<0){
	    printf("错误！帐号密码响应信息接收失败\n");
	    return -1;
	}
	if(strcmp(buffer,"错误")==0){
	    printf("帐号密码不存在\n");
	    return -1;
	}else{
	    printf("%s\n",buffer);
	}

	while(1){
       printf("**********************本系统有如下功能：********************\n");
       printf("\n");
       printf("1.指令：mkdir  创建目录               2.指令：rmdir  删除目录\n");
       printf("3.指令：cd     切换目录               4.指令：ls     显示目录\n");
       printf("5.指令：put    上传文件               6.指令：get    下载文件\n");
       printf("7.指令：exit   退出                                                                           \n");
	    printf("\n");
       printf("************************************************************\n");
	    printf("\n");
	    printf(">>>请输入指令:");
	    getstr(cmd);
	    printf("\n");
	    getcmd(hcmd,cmd);                                                    //
	    if(strcmp("mkdir",hcmd)==0){
	        if(write(fd_socket,cmd,sizeof(cmd))<0){
	            printf("错误！mkdir发送失败\n");
	            return -1;
	            }
            if(read(fd_socket,buffer,BUFFERSIZE)<0){
                printf("错误！ 信息接收失败：mkdir\n");
                return -1;
                }
            printf("%s\n",buffer);
	    }
	    else if(strcmp("rmdir",hcmd)==0){
            if(write(fd_socket,cmd,sizeof(cmd))<0){
	             printf("错误！rmdir发送失败\n");
	             return -1;
            }
            if(read(fd_socket,buffer,BUFFERSIZE)<0){
                printf("错误！ 信息接收失败：rmdir\n");
                return -1;
            }
            printf("%s\n",buffer);
	    }
	    else if(strcmp("cd",hcmd)==0){
            if(write(fd_socket,cmd,sizeof(cmd))<0){
	            printf("错误！cd发送失败\n");
	            return -1;
	            }
            if(read(fd_socket,buffer,BUFFERSIZE)<0){
                printf("错误！ 信息接收失败：cd\n");
                return -1;
                }
            printf("%s\n",buffer);
	    }
	    else if(strcmp("ls",hcmd)==0){
	         if(write(fd_socket,cmd,sizeof(cmd))<0){
	             printf("错误！ls发送失败\n");
	             return -1;
	            }

	         if(read(fd_socket,buffer,BUFFERSIZE)<0){
                printf("错误！ 信息接收失败：ls\n");
                return -1;
                }
            printf("%s\n",buffer);
	    }
	    else if(strcmp("put",hcmd)==0){
	        if(write(fd_socket,cmd,sizeof(cmd))<0){
	            printf("错误！put发送失败\n");
	            return -1;
	        }
	        printf("请输入上传文件的路径:");
            char localfile[512];
            getstr(localfile);
            printf("\n");
            FILE *file;
            bzero(temp,sizeof(temp));
            if((file = fopen(localfile,"r"))==NULL){
                printf("错误！fopen失败\n");
                return -1;
            }
            int num;
            num = fread(temp,1,sizeof(temp),file);
            write(fd_socket,temp,num);
            fclose(file);
	    }
	    else if(strcmp("get",hcmd)==0){
	        int fp;
            char localfile[512];
            int result;
	        if(write(fd_socket,cmd,sizeof(cmd))<0){
	            printf("错误！get发送失败\n");
	            return -1;
	        }
	        printf("请输入下载文件的存放路径：");
            getstr(localfile);
            printf("\n");
            if((fp = open(localfile,O_RDWR|O_CREAT|O_APPEND))<0){
                printf("错误！open失败\n");
                return -1;
            }
            result = read(fd_socket,buffer,sizeof(buffer));
            write(fp,buffer,result);
            close(fp);
            printf("接收%dB的文件\n",result);
	    }
	    else if(strcmp("exit",hcmd)==0){
	        if(write(fd_socket,cmd,sizeof(cmd))<0){
	            printf("错误！exit发送失败\n");
	            return -1;
	            }
	            close(fd_socket);
                exit(0);
	    }
	    else{
	        printf("无效指令，请重新输入\n");
	    }

	}
    return 0;
}
