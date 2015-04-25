#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXBUF 512

int main(int argc, char **argv)
{
int sockfd, len;
struct sockaddr_in server_addr;
char buffer[MAXBUF + 1];

if (argc != 3) {
printf("参数正确格式如下：\nIP地址 端口\n127.0.0.1 4567\n");
exit(0);
}
/* 创建一个 socket 用于 tcp 通信 */
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
perror("Socket");
exit(errno);
}
printf("socket created\n");

/* 初始化服务器端（对方）的地址和端口信息 */
bzero(&server_addr, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(atoi(argv[2]));
if (inet_aton(argv[1], (struct in_addr *) &server_addr.sin_addr.s_addr) == 0) {
perror(argv[1]);
exit(errno);
}

/* 连接服务器 */
if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
perror("Connect failure");
exit(errno);
}
printf("server connected\n");

/* 接收对方发过来的消息，最多接收 MAXBUF 个字节 */
bzero(buffer, MAXBUF + 1);
/* 接收服务器来的消息 */
len = recv(sockfd, buffer, MAXBUF, 0);
if(len > 0) 
printf("接收消息成功:'%s'，共%d个字节的数据\n", buffer, len);
else
   printf("消息接收失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));

bzero(buffer, MAXBUF + 1);
strcpy(buffer, "客户端发给服务器端的消息\n");
/* 发消息给服务器 */
len = send(sockfd, buffer, strlen(buffer), 0);
if(len < 0) 
printf("发送失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));
else 
printf("发送成功，共发送了%d个字节！\n", len);

/* 关闭连接 */
close(sockfd);
return 0;
}