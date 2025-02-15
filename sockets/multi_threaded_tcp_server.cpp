#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <pthread.h>

using namespace std;

const int PORT = 8080;

void* handle_client(void* arg) {
    int client_sockfd = *((int*)arg);
    char buffer[1024];

    // Communicate with the client

    int bytes_recieved = recv(client_sockfd, buffer, sizeof(buffer), 0);

    if(bytes_recieved <= 0) {
        cerr << "Client disconnected or recv failed" << endl;
        close(client_sockfd);
        return NULL;
    } else {
        buffer[bytes_recieved] = '\0';
        // Echo message back to the client
        cout << "Client message: " << buffer << endl;
        // Send response
        send(client_sockfd, buffer, sizeof(buffer) , 0);
    }
    // Client connection ending

    close(client_sockfd);
    pthread_exit(NULL);
}

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
    server_addr.sin_port = htons(PORT);

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

    while(true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);

        if(client_sockfd < 0) {
            perror("Accept failed");
            close(sockfd);
            continue;
        }

        pthread_t client_thread;

        if(pthread_create(&client_thread, NULL, handle_client, (void*)&client_sockfd) != 0) {
            cerr << "Failed to spawn thread" << endl;
            close(client_sockfd);
        } else {
            pthread_detach(client_thread);
        }
    }

    close(sockfd);
    return 0;
}
