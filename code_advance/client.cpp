#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

using namespace std;

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    try {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);  // 创建套接字
        if (sockfd == -1) {
            throw runtime_error("套接字创建失败...");
        }

        memset(&servaddr, 0, sizeof(servaddr));  // 初始化服务器地址结构体
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 设置服务器 IP 地址
        servaddr.sin_port = htons(8080);  // 设置服务器端口号

        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
            throw runtime_error("连接失败...");
        }

        char word[1024];
        memset(word, 0, sizeof(word));
        ssize_t recvBytes = recv(sockfd, word, sizeof(word), 0);  // 接收服务器发送的单词
        if (recvBytes <= 0) {
            throw runtime_error("从服务器接收单词失败...");
        }

        string cmd = "./word_count.sh " + string(word);  // 构建命令字符串
        cout << cmd << endl;
        system(cmd.c_str());  // 执行命令

        FILE* fp = popen(cmd.c_str(), "r");  // 执行命令并打开输出文件流
        if (fp == NULL) {
            throw runtime_error("执行命令失败: " + cmd);
        }

        char result[1024];
        memset(result, 0, sizeof(result));
        fgets(result, sizeof(result), fp);  // 读取命令执行结果
        pclose(fp);  // 关闭文件流

        ssize_t sendBytes = send(sockfd, result, strlen(result), 0);  // 将结果发送给服务器
        if (sendBytes <= 0) {
            throw runtime_error("向服务器发送结果失败...");
        }

        close(sockfd);  // 关闭套接字
    } catch (const exception& e) {
        cerr << "错误: " << e.what() << endl;
        if (sockfd != -1) {
            close(sockfd);
        }
        return 1;
    }

    return 0;
}
