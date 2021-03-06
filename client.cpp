#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const char* server_ip = "127.0.0.1";
uint16_t server_port = 1234;

int main()
{
    // 创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));           // 每个字节都用0填充
    server_addr.sin_family = AF_INET;                       // 使用IPv4地址
    server_addr.sin_addr.s_addr = inet_addr(server_ip);   // 具体的IP地址
    server_addr.sin_port = htons(server_port);                     // 端口
    connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));

    // 读取服务器传回的数据
    char buffer[40];
    read(sock, buffer, sizeof(buffer) - 1);

    printf("Message form server: %s\n", buffer);

    // 关闭套接字
    close(sock);

    return 0;
}