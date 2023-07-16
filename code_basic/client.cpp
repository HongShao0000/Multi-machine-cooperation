#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<cstring>
#include<cstdlib>

using namespace std;

int main(){
    int sockfd, connfd;  // 客户端套接字的文件描述符
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

    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){  // 连接到服务器
        cout << "连接失败...\n";
        return 0;
    }

    char word[1024];
    recv(sockfd, word, sizeof(word), 0);  // 接收服务器发送的消息

    string cmd1("./word_count.sh");  // 构建命令字符串
    cout << cmd1 << endl;
    string cmd2 = cmd1 + word;
    cout << cmd2 << endl;
    string cmd = string("./word_count.sh ") + " " + word;
    cout << cmd << endl;
    system(cmd.c_str());  // 执行命令
    close(sockfd);  // 关闭套接字
    return 0;
}


/*创建客户端套接字 sockfd 用于与服务器进行通信。
设置套接字选项 SO_REUSEADDR，允许重新使用绑定的地址。
初始化服务器地址结构体 servaddr，设置服务器的 IP 地址和端口号。
连接到服务器，使用 connect 函数将套接字连接到服务器。
接收服务器发送的消息，使用 recv 函数从服务器接收消息并存储在 word 字符数组中。
构建命令字符串，拼接命令和接收到的消息。
执行命令，使用 system 函数执行命令字符串。
关闭客户端套接字，使用 close 函数关闭套接字。
返回 0 表示程序正常结束。
这段代码实现了一个简单的客户端，它连接到服务器并执行命令。
客户端从服务器接收到一个消息（word）后，构建一个命令字符串，将命令和消息拼接在一起，
然后使用 system 函数执行该命令。*/