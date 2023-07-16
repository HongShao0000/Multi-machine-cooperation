#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    // 创建套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cout << "套接字创建失败..." << endl;
        return 0;
    }

    // 初始化服务器地址结构体
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 设置服务器 IP 地址
    servaddr.sin_port = htons(8080);  // 设置服务器端口号

    // 连接到服务器
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        cout << "连接失败..." << endl;
        return 0;
    }

    char word[1024];
    recv(sockfd, word, sizeof(word), 0);  // 接收服务器发送的消息

    string cmd = "./word_count.sh " + string(word);  // 构建命令字符串
    system(cmd.c_str());  // 执行命令

    close(sockfd);  // 关闭套接字
    return 0;
}
