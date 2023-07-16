#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

using namespace std;

int main() {
    int sockfd, connfd;// �������׽��ֺͿͻ����׽��ֵ��ļ�������
    struct sockaddr_in servaddr, clientaddr;// �洢�������Ϳͻ��˵�ַ��Ϣ�Ľṹ��
    socklen_t len = sizeof(clientaddr);// �ͻ��˵�ַ�ṹ��ĳ���
    // ����һ���׽���
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cout << "Socket creation failed..." << endl;
        return 0;
    }
    // �����׽���ѡ�������õ�ַ
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // ��ʼ����������ַ�ṹ��
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);

    // ���׽��ְ󶨵�ָ����ַ�Ͷ˿�
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        cout << "Socket bind failed..." << endl;
        return 0;
    }

    // ��ʼ������������
    if (listen(sockfd, 5) != 0) {
        cout << "Listen failed..." << endl;
        return 0;
    }

    cout << "Server is running and waiting for connections..." << endl;

    vector<int> clientSockets;

    while (true) {
        connfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);//���ܿͻ��˵���������
        if (connfd < 0) {
            cout << "Server accept failed..." << endl;
            return 0;
        }

        cout << "Client connected. Socket ID: " << connfd << endl;
        clientSockets.push_back(connfd);// ���ͻ����׽�����ӵ��б���

        if (clientSockets.size() == 2) {  // ����ֻ�������ͻ�������
            int n, m;
            string word;

            cout << "Enter the total number of files (n): ";
            cin >> n;
            cout << "Enter the total number of clients (m): ";
            cin >> m;
            cout << "Enter the word to count: ";
            cin.ignore();
            getline(cin, word);

            int filesPerClient = n / m;// ÿ���ͻ��˴�����ļ�����
            int remainingFiles = n % m;// ʣ����ļ�����


            for (int i = 0; i < clientSockets.size(); i++) {
                int startFile = i * filesPerClient + 1;// ��ʼ�ļ����
                int endFile = (i + 1) * filesPerClient;// �����ļ����
                if (i == clientSockets.size() - 1) {
                    endFile += remainingFiles;// ���һ���ͻ��˴���ʣ����ļ�
                }

                string message = to_string(startFile) + " " + to_string(endFile) + " \"" + word + "\"";// ������Ϣ�ַ���
                send(clientSockets[i], message.c_str(), message.length(), 0);// ������Ϣ���ͻ���
                cout << "Message sent to client " << i+1 << ": " << message << endl;
            }

            break;
        }
    }

    // �رտͻ����׽���
    for (int i = 0; i < clientSockets.size(); i++) {
        close(clientSockets[i]);
    }
    
    // �رշ������׽���
    close(sockfd);
    return 0;
}
