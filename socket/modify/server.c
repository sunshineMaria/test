#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <netdb.h>  
#include <string.h>  
#include <arpa/inet.h>  
int main()  
{  
    int sockfd_server;  
    int sockfd_client;  
    struct sockaddr_in addr_server;  
    struct sockaddr_in addr_client;  
    socklen_t addr_len = 0;  
    int client_max = 10;  
    int server_port = 1111;  
  
    srand(time(NULL));  
      
    // create socket  
    sockfd_server = socket(AF_INET, SOCK_STREAM, 0);  
    if(sockfd_server == -1)  
    {  
        printf("init socket failed\n");  
        return -1;  
    }  
  
    // set address  
    memset(&addr_server, 0, sizeof(addr_server));  
    addr_server.sin_family = AF_INET;  
    addr_server.sin_addr.s_addr = htonl(INADDR_ANY);  
    addr_server.sin_port = htons(server_port);  
      
    // socket bind with address  
    if(bind(sockfd_server, (struct sockaddr*)&addr_server, sizeof(struct sockaddr)) == -1)  
    {  
        printf("bind socket failed\n");  
        return -1;  
    }  
  
    // server socket start list, waitting client to connect  
    // 这个client_max是指同时连接数  
    if(listen(sockfd_server, client_max) == -1)  
    {  
        printf("start listen socket failed\n");  
        return -1;  
    }  
    char szMsg[128] = {0};  
    while(1)  
    {  
        addr_len = sizeof(struct sockaddr_in);  
        printf("waitting for connected...\n");  
          
        // waitting for connected  
        sockfd_client = accept(sockfd_server, (struct sockaddr*)&addr_client, &addr_len);  
        if(sockfd_client == -1)  
        {  
            printf("accept socket failed\n");  
            return -1;  
        }  
        printf("accept success");
        // get client ip and send a magic number to the client,   
        // then close this connection  
        printf("recived connection from : %s\n", inet_ntoa(addr_client.sin_addr));  
        sprintf(szMsg, "Your magic number is : %d", rand()%100);  
        write(sockfd_client, szMsg, strlen(szMsg));  
  
        close(sockfd_client);  
    }  
    close(sockfd_server);  
    return 0;  
}  