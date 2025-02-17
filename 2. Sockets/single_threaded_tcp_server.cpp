#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace std;

int main() {

    // Creating the socket

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0) {
        cerr << "Error opening socket" << endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Binding the socket

    if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind falied");
        close(sockfd);
        return 1;
    }

    cout << "Socket succesfully bound to port 8080" << endl;

    // Listening

    if(listen(sockfd, SOMAXCONN) < 0) {
        perror("Listen failed");
        close(sockfd);
        return 1;
    }

    // Accepting connections

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
    if(client_sockfd < 0) {
        perror("Accept failed");
        close(sockfd);
        return 1;
    }

    cout << "Connection established with client" << endl;

    close(sockfd);
    return 0;
}
