#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int main(){
    int sockfd, connfd;  // 服务器套接字和客户端套接字的文件描述符
    struct sockaddr_in servaddr, clientaddr;  // 存储服务器和客户端地址信息的结构体
    socklen_t len = sizeof(clientaddr);  // 客户端地址结构体的长度

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // 创建套接字
    if(sockfd == -1){
        cout << "套接字创建失败...\n";
        return 0;    
    }
    
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  // 设置套接字选项以重用地址

    memset(&servaddr, 0, sizeof(servaddr));  // 初始化服务器地址结构体
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 设置服务器 IP 地址
    servaddr.sin_port = htons(8080);  // 设置服务器端口号

    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0){  // 将套接字绑定到指定地址和端口
        cout << "套接字绑定失败...\n";
        return 0;
    }

    if(listen(sockfd, 5) != 0){  // 开始监听连接请求，5 表示可以同时处理的最大连接数
        cout << "监听失败...\n";
        return 0;
    }

    connfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);  // 接受客户端的连接请求
    if(connfd < 0){
        cout << "服务器接受连接失败...\n";
        return 0;
    }

    int a, b;
    string c;
    cout << "请输入 a 的值：";
    cin >> a;
    cout << "请输入 b 的值：";
    cin >> b;
    cout << "请输入 c 的值：";
    cin.ignore();
    getline(cin, c);

    string message = to_string(a) + " " + to_string(b) + " \"" + c + "\"";
    send(connfd, message.c_str(), message.length(), 0);  // 发送消息给客户端
    cout << "已发送消息给客户端：" << message <<endl;

    close(sockfd);  // 关闭服务器套接字
    return 0;
}

/*创建套接字 sockfd 用于与客户端进行通信。
设置套接字选项 SO_REUSEADDR，允许重新使用绑定的地址。
初始化服务器地址结构体 servaddr，设置服务器的 IP 地址和端口号。
将套接字绑定到服务器地址。
开始监听连接请求，listen 函数的第二个参数 5 表示可以同时处理的最大连接数。
接受客户端的连接请求，返回一个新的套接字 connfd，用于与客户端进行通信。
提示用户输入变量 a、b 和 c 的值。
构建消息字符串。
使用 send 函数将消息发送给客户端。
打印已发送的消息。
关闭服务器套接字。
返回 0 表示程序正常结束。
这段代码实现了一个简单的服务器，它接受客户端的连接并向其发送消息，传递了变量 a、b 和 c 的值。*/