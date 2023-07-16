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
    int sockfd, connfd;  // �������׽��ֺͿͻ����׽��ֵ��ļ�������
    struct sockaddr_in servaddr, clientaddr;  // �洢�������Ϳͻ��˵�ַ��Ϣ�Ľṹ��
    socklen_t len = sizeof(clientaddr);  // �ͻ��˵�ַ�ṹ��ĳ���

    try {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);  // �����׽���
        if (sockfd == -1) {
            throw runtime_error("�׽��ִ���ʧ��...");
        }

        int opt = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  // �����׽���ѡ�������õ�ַ

        memset(&servaddr, 0, sizeof(servaddr));  // ��ʼ����������ַ�ṹ��
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(8080);

        if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
            throw runtime_error("�׽��ְ�ʧ��...");
        }

        if (listen(sockfd, 5) != 0) {
            throw runtime_error("����ʧ��...");
        }

        cout << "�������������в��ȴ�����..." << endl;

        vector<int> clientSockets;  // �洢�ͻ����׽��ֵ��ļ�������
        int m;
        cout << "������ͻ���������m����";
        cin >> m;

        while (true) {
            connfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);  // ���ܿͻ��˵���������
            if (connfd < 0) {
                throw runtime_error("��������������ʧ��...");
            }

            cout << "�ͻ��������ӡ��׽���ID��" << connfd << endl;
            clientSockets.push_back(connfd);  // ���ͻ����׽�����ӵ��б���

            if (clientSockets.size() == m) {  // ����ֻ�� m ���ͻ�������
                int n;
                string word;

                cout << "�������ļ�������n����";
                cin >> n;

                cout << "������Ҫͳ�Ƶĵ��ʣ�";
                cin.ignore();
                getline(cin, word);

                int filesPerClient = n / m;  // ÿ���ͻ��˴�����ļ�����
                int remainingFiles = n % m;  // ʣ����ļ�����

                for (int i = 0; i < clientSockets.size(); i++) {
                    int startFile = i * filesPerClient + 1;  // ��ʼ�ļ����
                    int endFile = (i + 1) * filesPerClient;  // �����ļ����
                    if (i == clientSockets.size() - 1) {
                        endFile += remainingFiles;  // ���һ���ͻ��˴���ʣ����ļ�
                    }

                    string message = to_string(startFile) + " " + to_string(endFile) + " \"" + word + "\"";  // ������Ϣ�ַ���
                    ssize_t sendBytes = send(clientSockets[i], message.c_str(), message.length(), 0);  // ������Ϣ���ͻ���
                    if (sendBytes <= 0) {
                        throw runtime_error("��ͻ���" + to_string(i+1) + "������Ϣʧ��");
                    }
                    cout << "�ѷ�����Ϣ���ͻ���" << i+1 << "��" << message << endl;
                }

                int totalOccurrences = 0;
                for (int i = 0; i < clientSockets.size(); i++) {
                    char buffer[1024];
                    memset(buffer, 0, sizeof(buffer));
                    ssize_t recvBytes = recv(clientSockets[i], buffer, sizeof(buffer), 0);  // �������Կͻ��˵Ľ��
                    if (recvBytes <= 0) {
                        throw runtime_error("�ӿͻ���" + to_string(i+1) + "���ս��ʧ��");
                    }
                    int occurrences = atoi(buffer);
                    totalOccurrences += occurrences;
                }
                
                ofstream outfile("output.txt");  // ������ļ�
                streambuf* coutBuf = cout.rdbuf();
                cout.rdbuf(outfile.rdbuf());  // �ض���������ļ�
                cout << "����'" << word << "'���ļ�1���ļ�" << n << "�г�����" << totalOccurrences << "��" << endl;
                cout.rdbuf(coutBuf);  // �ָ����������̨
                outfile.close();  // �ر�����ļ�
                break;
            }
        }

        for (int i = 0; i < clientSockets.size(); i++) {
            close(clientSockets[i]);  // �رտͻ����׽���
        }

        close(sockfd);  // �رշ������׽���
    } catch (const exception& e) {
        cerr << "����" << e.what() << endl;
        if (sockfd != -1) {
            close(sockfd);
        }
        return 1;
    }

    return 0;
}
