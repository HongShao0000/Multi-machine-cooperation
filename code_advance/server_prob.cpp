// 服务器端代码
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

using namespace std;

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t len = sizeof(clientaddr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cout << "Socket creation failed..." << endl;
        return 0;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        cout << "Socket bind failed..." << endl;
        return 0;
    }

    if (listen(sockfd, 5) != 0) {
        cout << "Listen failed..." << endl;
        return 0;
    }

    cout << "Server is running and waiting for connections..." << endl;

    vector<int> clientSockets;

    while (true) {
        connfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
        if (connfd < 0) {
            cout << "Server accept failed..." << endl;
            return 0;
        }

        cout << "Client connected. Socket ID: " << connfd << endl;
        clientSockets.push_back(connfd);

        if (clientSockets.size() == 2) {  // Assuming there will be exactly 2 clients
            int n, m;
            string word;

            cout << "Enter the total number of files (n): ";
            cin >> n;
            cout << "Enter the total number of clients (m): ";
            cin >> m;
            cout << "Enter the word to count: ";
            cin.ignore();
            getline(cin, word);

            int filesPerClient = n / m;
            int remainingFiles = n % m;

            for (int i = 0; i < clientSockets.size(); i++) {
                int startFile = i * filesPerClient + 1;
                int endFile = (i + 1) * filesPerClient;
                if (i == clientSockets.size() - 1) {
                    endFile += remainingFiles;
                }

                string message = to_string(startFile) + " " + to_string(endFile) + " \"" + word + "\"";
                send(clientSockets[i], message.c_str(), message.length(), 0);
                cout << "Message sent to client " << i+1 << ": " << message << endl;
            }

            int totalOccurrences = 0;
            for (int i = 0; i < clientSockets.size(); i++) {
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                recv(clientSockets[i], buffer, sizeof(buffer), 0);
                int occurrences = atoi(buffer);
                totalOccurrences += occurrences;
            }

            cout << "Word '" << word << "' appears " << totalOccurrences << " times from file No.1 to file No." << n << endl;

            break;
        }
    }

    for (int i = 0; i < clientSockets.size(); i++) {
        close(clientSockets[i]);
    }

    close(sockfd);
    return 0;
}

