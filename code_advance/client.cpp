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
        sockfd = socket(AF_INET, SOCK_STREAM, 0);  // �����׽���
        if (sockfd == -1) {
            throw runtime_error("�׽��ִ���ʧ��...");
        }

        memset(&servaddr, 0, sizeof(servaddr));  // ��ʼ����������ַ�ṹ��
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // ���÷����� IP ��ַ
        servaddr.sin_port = htons(8080);  // ���÷������˿ں�

        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
            throw runtime_error("����ʧ��...");
        }

        char word[1024];
        memset(word, 0, sizeof(word));
        ssize_t recvBytes = recv(sockfd, word, sizeof(word), 0);  // ���շ��������͵ĵ���
        if (recvBytes <= 0) {
            throw runtime_error("�ӷ��������յ���ʧ��...");
        }

        string cmd = "./word_count.sh " + string(word);  // ���������ַ���
        cout << cmd << endl;
        system(cmd.c_str());  // ִ������

        FILE* fp = popen(cmd.c_str(), "r");  // ִ�����������ļ���
        if (fp == NULL) {
            throw runtime_error("ִ������ʧ��: " + cmd);
        }

        char result[1024];
        memset(result, 0, sizeof(result));
        fgets(result, sizeof(result), fp);  // ��ȡ����ִ�н��
        pclose(fp);  // �ر��ļ���

        ssize_t sendBytes = send(sockfd, result, strlen(result), 0);  // ��������͸�������
        if (sendBytes <= 0) {
            throw runtime_error("����������ͽ��ʧ��...");
        }

        close(sockfd);  // �ر��׽���
    } catch (const exception& e) {
        cerr << "����: " << e.what() << endl;
        if (sockfd != -1) {
            close(sockfd);
        }
        return 1;
    }

    return 0;
}
