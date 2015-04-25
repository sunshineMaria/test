#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXBUF 512

int main(int argc, char **argv)
{
int sockfd, new_fd;
socklen_t len;
struct sockaddr_in server_addr, client_addr;
unsigned int port, num;
char buf[MAXBUF + 1];

if (argv[1])
port = atoi(argv[1]);
else
port = 5678;

if (argv[2])
num = atoi(argv[2]);
else
num = 3;

if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
perror("socket");
exit(1);
}
else printf("socket created success!!!\n");

bzero(&server_addr, sizeof(server_addr));
server_addr.sin_family = PF_INET;
server_addr.sin_port = htons(port);
if(argv[3]) 
server_addr.sin_addr.s_addr = inet_addr(argv[3]);
else 
server_addr.sin_addr.s_addr = INADDR_ANY;

if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1) {
perror("bind");
exit(1);
}else 
printf("binded success!!!\n");

if (listen(sockfd, num) == -1) {
perror("listen");
exit(1);
}else 
printf("Let's listen\n");

while(1) {
len = sizeof(struct sockaddr);
if ((new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &len)) == -1) {
perror("accept");
exit(errno);
}else 
printf("client connect the server now: client %s, port %d, new socket %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), new_fd);

/* 开始处理每个新连接上的数据收发 */
bzero(buf, MAXBUF + 1);
strcpy(buf, "服务器发给客户端的消息\n只能向new_fd这个用accept函数新建立的socket发消息，不能向sockfd这个监听socket发送消息，监听socket不能用来接收或发送消息\n");
/* 发消息给客户端 */
len = send(new_fd, buf, strlen(buf), 0);
if(len < 0) {
printf("消息发送失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));
}else 
printf("消息发送成功，共发送了%d个字节！\n", len);

bzero(buf, MAXBUF + 1);
/* 接收客户端的消息 */
len = recv(new_fd, buf, MAXBUF, 0);
if(len > 0)
printf("接收成功:'%s'，共%d个字节的数据\n", buf, len);
else 
printf("接收失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));
/* 处理每个新连接上的数据收发结束 */
}

close(sockfd);
return 0;
}