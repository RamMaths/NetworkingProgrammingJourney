#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

using namespace std;

const int PORT = 8080;
const char *SERVER_IP = "127.0.0.1";

int main() {
    // Setting up the server we want to make requests to
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Creating the socket
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        cerr << "Invalid server address" << endl;
        close(sockfd);
        return -1;
    }

    if(connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server falied");
        close(sockfd);
        return -1;
    }

    cout << "Connected to the server" << endl;

    // Send a message to the server
    char buffer[1024] = "Hello, Server!";
    send(sockfd, buffer, strlen(buffer), 0);

    // Recieve a response from the server
    int bytes_recieved = recv(sockfd, buffer, sizeof(buffer)-1, 0);

    if(bytes_recieved > 0) {
        cout << "Server reply: " << buffer << endl;
    } else {
        cerr << "Server response failed" << endl;
    }

    close(sockfd);
    return 0;
}
