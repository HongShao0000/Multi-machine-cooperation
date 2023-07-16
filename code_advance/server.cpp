#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <stdexcept>
#include <fstream>

using namespace std;

int main() {
    int sockfd, connfd;  // 服务器套接字和客户端套接字的文件描述符
    struct sockaddr_in servaddr, clientaddr;  // 存储服务器和客户端地址信息的结构体
    socklen_t len = sizeof(clientaddr);  // 客户端地址结构体的长度

    try {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);  // 创建套接字
        if (sockfd == -1) {
            throw runtime_error("套接字创建失败...");
        }

        int opt = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  // 设置套接字选项以重用地址

        memset(&servaddr, 0, sizeof(servaddr));  // 初始化服务器地址结构体
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(8080);

        if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
            throw runtime_error("套接字绑定失败...");
        }

        if (listen(sockfd, 5) != 0) {
            throw runtime_error("监听失败...");
        }

        cout << "服务器正在运行并等待连接..." << endl;

        vector<int> clientSockets;  // 存储客户端套接字的文件描述符
        int m;
        cout << "请输入客户端总数（m）：";
        cin >> m;

        while (true) {
            connfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);  // 接受客户端的连接请求
            if (connfd < 0) {
                throw runtime_error("服务器接受连接失败...");
            }

            cout << "客户端已连接。套接字ID：" << connfd << endl;
            clientSockets.push_back(connfd);  // 将客户端套接字添加到列表中

            if (clientSockets.size() == m) {  // 假设只有 m 个客户端连接
                int n;
                string word;

                cout << "请输入文件总数（n）：";
                cin >> n;

                cout << "请输入要统计的单词：";
                cin.ignore();
                getline(cin, word);

                int filesPerClient = n / m;  // 每个客户端处理的文件数量
                int remainingFiles = n % m;  // 剩余的文件数量

                for (int i = 0; i < clientSockets.size(); i++) {
                    int startFile = i * filesPerClient + 1;  // 起始文件编号
                    int endFile = (i + 1) * filesPerClient;  // 结束文件编号
                    if (i == clientSockets.size() - 1) {
                        endFile += remainingFiles;  // 最后一个客户端处理剩余的文件
                    }

                    string message = to_string(startFile) + " " + to_string(endFile) + " \"" + word + "\"";  // 构建消息字符串
                    ssize_t sendBytes = send(clientSockets[i], message.c_str(), message.length(), 0);  // 发送消息给客户端
                    if (sendBytes <= 0) {
                        throw runtime_error("向客户端" + to_string(i+1) + "发送消息失败");
                    }
                    cout << "已发送消息给客户端" << i+1 << "：" << message << endl;
                }

                int totalOccurrences = 0;
                for (int i = 0; i < clientSockets.size(); i++) {
                    char buffer[1024];
                    memset(buffer, 0, sizeof(buffer));
                    ssize_t recvBytes = recv(clientSockets[i], buffer, sizeof(buffer), 0);  // 接收来自客户端的结果
                    if (recvBytes <= 0) {
                        throw runtime_error("从客户端" + to_string(i+1) + "接收结果失败");
                    }
                    int occurrences = atoi(buffer);
                    totalOccurrences += occurrences;
                }
                
                ofstream outfile("output.txt");  // 打开输出文件
                streambuf* coutBuf = cout.rdbuf();
                cout.rdbuf(outfile.rdbuf());  // 重定向输出到文件
                cout << "单词'" << word << "'在文件1到文件" << n << "中出现了" << totalOccurrences << "次" << endl;
                cout.rdbuf(coutBuf);  // 恢复输出到控制台
                outfile.close();  // 关闭输出文件
                break;
            }
        }

        for (int i = 0; i < clientSockets.size(); i++) {
            close(clientSockets[i]);  // 关闭客户端套接字
        }

        close(sockfd);  // 关闭服务器套接字
    } catch (const exception& e) {
        cerr << "错误：" << e.what() << endl;
        if (sockfd != -1) {
            close(sockfd);
        }
        return 1;
    }

    return 0;
}
