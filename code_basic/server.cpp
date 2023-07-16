#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int main(){
    int sockfd, connfd;  // �������׽��ֺͿͻ����׽��ֵ��ļ�������
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

    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0){  // ���׽��ְ󶨵�ָ����ַ�Ͷ˿�
        cout << "�׽��ְ�ʧ��...\n";
        return 0;
    }

    if(listen(sockfd, 5) != 0){  // ��ʼ������������5 ��ʾ����ͬʱ��������������
        cout << "����ʧ��...\n";
        return 0;
    }

    connfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);  // ���ܿͻ��˵���������
    if(connfd < 0){
        cout << "��������������ʧ��...\n";
        return 0;
    }

    int a, b;
    string c;
    cout << "������ a ��ֵ��";
    cin >> a;
    cout << "������ b ��ֵ��";
    cin >> b;
    cout << "������ c ��ֵ��";
    cin.ignore();
    getline(cin, c);

    string message = to_string(a) + " " + to_string(b) + " \"" + c + "\"";
    send(connfd, message.c_str(), message.length(), 0);  // ������Ϣ���ͻ���
    cout << "�ѷ�����Ϣ���ͻ��ˣ�" << message <<endl;

    close(sockfd);  // �رշ������׽���
    return 0;
}

/*�����׽��� sockfd ������ͻ��˽���ͨ�š�
�����׽���ѡ�� SO_REUSEADDR����������ʹ�ð󶨵ĵ�ַ��
��ʼ����������ַ�ṹ�� servaddr�����÷������� IP ��ַ�Ͷ˿ںš�
���׽��ְ󶨵���������ַ��
��ʼ������������listen �����ĵڶ������� 5 ��ʾ����ͬʱ����������������
���ܿͻ��˵��������󣬷���һ���µ��׽��� connfd��������ͻ��˽���ͨ�š�
��ʾ�û�������� a��b �� c ��ֵ��
������Ϣ�ַ�����
ʹ�� send ��������Ϣ���͸��ͻ��ˡ�
��ӡ�ѷ��͵���Ϣ��
�رշ������׽��֡�
���� 0 ��ʾ��������������
��δ���ʵ����һ���򵥵ķ������������ܿͻ��˵����Ӳ����䷢����Ϣ�������˱��� a��b �� c ��ֵ��*/