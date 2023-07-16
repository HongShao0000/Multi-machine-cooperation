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

    // �����׽���
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cout << "�׽��ִ���ʧ��..." << endl;
        return 0;
    }

    // ��ʼ����������ַ�ṹ��
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // ���÷����� IP ��ַ
    servaddr.sin_port = htons(8080);  // ���÷������˿ں�

    // ���ӵ�������
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        cout << "����ʧ��..." << endl;
        return 0;
    }

    char word[1024];
    recv(sockfd, word, sizeof(word), 0);  // ���շ��������͵���Ϣ

    string cmd = "./word_count.sh " + string(word);  // ���������ַ���
    system(cmd.c_str());  // ִ������

    close(sockfd);  // �ر��׽���
    return 0;
}
