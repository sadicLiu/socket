#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <opencv2/opencv.hpp>
#include "display.h"

using namespace std;
using namespace cv;

#define NUM_CLASSES 9

// 对输入图片进行分类, 获取预测结果
// TODO: 目前是模拟的结果, 分类可用之后, 使用真实分类流程替代
float *predict()
{
    float *pred = new float[NUM_CLASSES];
    float small[7] = {0.01, 0.0, 0.01, 0.01, 0.02, 0.03, 0.02};

    srand((unsigned) time(NULL));
    float top1 = (rand() % 10) / 10.f;
    float top2 = 0.9f - top1;

    srand((unsigned) time(NULL));
    int index_top1 = rand() % 9;
    int index_top2 = index_top1 / 2 == index_top1 ? index_top1 + 1 : index_top1 / 2;

    int idx_small = 0;
    int i;
    for (i = 0; i < NUM_CLASSES; i++)
    {
        pred[i] = i == index_top1 ? top1
                                  : i == index_top2 ? top2
                                                    : small[idx_small++];
    }

    return pred;
}

// 分类一张图片的demo
void run_demo()
{
    string img_path = "./tank.jpg";
    Mat img = imread(img_path);
    resize(img, img, Size(640, 480));

    // float *pred = predict(network, img);  // todo: invoke network prediction here, 224x224 input
    float *pred = predict();
    show_result(img, pred, TOPK, true, true);
    delete[]pred;
}


const char *ip = "127.0.0.1";
uint16_t port = 1234;


int main()
{
    // 创建套接字, 确定套接字的各种属性
    // AF_INET表示使用 IPv4 地址, SOCK_STREAM 表示使用面向连接的数据传输方式, IPPROTO_TCP 表示使用 TCP 协议
    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 将套接字和IP、端口绑定
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));            // 每个字节都用0填充
    server_addr.sin_family = AF_INET;                      // 使用IPv4地址
    server_addr.sin_addr.s_addr = inet_addr(ip);  // 具体的IP地址
    server_addr.sin_port = htons(port);                    // 端口

    // 让套接字与特定的IP地址和端口对应起来，这样客户端才能连接到该套接字。
    bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    cout << "Server start success, now listening on port " << port << endl;

    while (true)
    {
        // 进入监听状态(被动监听, 套接字一直处于睡眠中, 直到客户端发起请求才会被唤醒), 等待用户发起请求
        listen(server_sock, 20);

        // 接收客户端请求
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);

        // run_demo();

        // 向客户端发送数据
        char str[] = "Done";
        write(client_sock, str, sizeof(str));
        close(client_sock);
    }


    // 关闭套接字
    close(server_sock);

    return 0;
}