#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <netdb.h>  
#include <string.h>  
#include <arpa/inet.h>  
int main()  
{  
    int sockfd_server;  
    struct sockaddr_in addr_server;  
    int server_port = 1111;  
    char* server_addr = "127.0.0.1";  
  
  
    // create socket  
    sockfd_server = socket(AF_INET, SOCK_STREAM, 0);  
    if(sockfd_server == -1)  
    {  
        printf("init socket failed\n");  
        return -1;  
    }  
      
    // set server address  
    memset(&addr_server, 0, sizeof(addr_server));  
    addr_server.sin_family = AF_INET;  
    addr_server.sin_addr.s_addr = inet_addr(server_addr);;  
    addr_server.sin_port = htons(server_port);  
      
    // connect server  
    if(connect(sockfd_server,(struct sockaddr *)&addr_server,sizeof(struct sockaddr))==-1)  
    {  
        printf("connect server failed\n");  
        return -1;  
    }
    printf("connect success\n");
      
    char szBuf[512] = {0};  
    int nRead = 0;  
      
    // recived data from server and print it  
    nRead = read(sockfd_server, szBuf, sizeof(szBuf));  
    printf("recived from server : %s\n", szBuf);      
      
    close(sockfd_server);  
    return 0;  
}  