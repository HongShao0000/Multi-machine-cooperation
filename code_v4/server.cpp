#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

using namespace std;

int main() {
    int sockfd, connfd;// 服务器套接字和客户端套接字的文件描述符
    struct sockaddr_in servaddr, clientaddr;// 存储服务器和客户端地址信息的结构体
    socklen_t len = sizeof(clientaddr);// 客户端地址结构体的长度
    // 创建一个套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cout << "Socket creation failed..." << endl;
        return 0;
    }
    // 设置套接字选项以重用地址
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // 初始化服务器地址结构体
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);

    // 将套接字绑定到指定地址和端口
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        cout << "Socket bind failed..." << endl;
        return 0;
    }

    // 开始监听连接请求
    if (listen(sockfd, 5) != 0) {
        cout << "Listen failed..." << endl;
        return 0;
    }

    cout << "Server is running and waiting for connections..." << endl;

    vector<int> clientSockets;

    while (true) {
        connfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);//接受客户端的连接请求
        if (connfd < 0) {
            cout << "Server accept failed..." << endl;
            return 0;
        }

        cout << "Client connected. Socket ID: " << connfd << endl;
        clientSockets.push_back(connfd);// 将客户端套接字添加到列表中

        if (clientSockets.size() == 2) {  // 假设只有两个客户端连接
            int n, m;
            string word;

            cout << "Enter the total number of files (n): ";
            cin >> n;
            cout << "Enter the total number of clients (m): ";
            cin >> m;
            cout << "Enter the word to count: ";
            cin.ignore();
            getline(cin, word);

            int filesPerClient = n / m;// 每个客户端处理的文件数量
            int remainingFiles = n % m;// 剩余的文件数量


            for (int i = 0; i < clientSockets.size(); i++) {
                int startFile = i * filesPerClient + 1;// 起始文件编号
                int endFile = (i + 1) * filesPerClient;// 结束文件编号
                if (i == clientSockets.size() - 1) {
                    endFile += remainingFiles;// 最后一个客户端处理剩余的文件
                }

                string message = to_string(startFile) + " " + to_string(endFile) + " \"" + word + "\"";// 构建消息字符串
                send(clientSockets[i], message.c_str(), message.length(), 0);// 发送消息给客户端
                cout << "Message sent to client " << i+1 << ": " << message << endl;
            }

            break;
        }
    }

    // 关闭客户端套接字
    for (int i = 0; i < clientSockets.size(); i++) {
        close(clientSockets[i]);
    }
    
    // 关闭服务器套接字
    close(sockfd);
    return 0;
}
