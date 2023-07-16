// 客户端代码
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

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cout << "Socket creation failed..." << endl;
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(8080);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        cout << "Connect failed..." << endl;
        return 0;
    }

    char word[1024];
    recv(sockfd, word, sizeof(word), 0);

    string cmd = "./word_count.sh " + string(word);
    cout << cmd << endl;
    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == NULL) {
        cout << "Error executing command: " << cmd << endl;
        close(sockfd);
        return 0;
    }

    char result[1024];
    memset(result, 0, sizeof(result));
    fgets(result, sizeof(result), fp);
    pclose(fp);

    send(sockfd, result, strlen(result), 0);

    close(sockfd);
    return 0;
}

