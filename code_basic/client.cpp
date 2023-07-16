#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<cstring>
#include<cstdlib>

using namespace std;

int main(){
    int sockfd, connfd;  // �ͻ����׽��ֵ��ļ�������
    struct sockaddr_in servaddr, clientaddr;  // �洢�������Ϳͻ��˵�ַ��Ϣ�Ľṹ��
    socklen_t len = sizeof(clientaddr);  // �ͻ��˵�ַ�ṹ��ĳ���

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // �����׽���
    if(sockfd == -1){
        cout << "�׽��ִ���ʧ��...\n";
        return 0;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  // �����׽���ѡ�������õ�ַ

    memset(&servaddr, 0, sizeof(servaddr));  // ��ʼ����������ַ�ṹ��
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // ���÷����� IP ��ַ
    servaddr.sin_port = htons(8080);  // ���÷������˿ں�

    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){  // ���ӵ�������
        cout << "����ʧ��...\n";
        return 0;
    }

    char word[1024];
    recv(sockfd, word, sizeof(word), 0);  // ���շ��������͵���Ϣ

    string cmd1("./word_count.sh");  // ���������ַ���
    cout << cmd1 << endl;
    string cmd2 = cmd1 + word;
    cout << cmd2 << endl;
    string cmd = string("./word_count.sh ") + " " + word;
    cout << cmd << endl;
    system(cmd.c_str());  // ִ������
    close(sockfd);  // �ر��׽���
    return 0;
}


/*�����ͻ����׽��� sockfd ���������������ͨ�š�
�����׽���ѡ�� SO_REUSEADDR����������ʹ�ð󶨵ĵ�ַ��
��ʼ����������ַ�ṹ�� servaddr�����÷������� IP ��ַ�Ͷ˿ںš�
���ӵ���������ʹ�� connect �������׽������ӵ���������
���շ��������͵���Ϣ��ʹ�� recv �����ӷ�����������Ϣ���洢�� word �ַ������С�
���������ַ�����ƴ������ͽ��յ�����Ϣ��
ִ�����ʹ�� system ����ִ�������ַ�����
�رտͻ����׽��֣�ʹ�� close �����ر��׽��֡�
���� 0 ��ʾ��������������
��δ���ʵ����һ���򵥵Ŀͻ��ˣ������ӵ���������ִ�����
�ͻ��˴ӷ��������յ�һ����Ϣ��word���󣬹���һ�������ַ��������������Ϣƴ����һ��
Ȼ��ʹ�� system ����ִ�и����*/