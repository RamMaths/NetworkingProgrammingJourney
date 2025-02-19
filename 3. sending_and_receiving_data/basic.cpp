#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

int sendData(int socket_fd, const char* message) {
    int total_sent = 0;
    int bytes_left = strlen(message);
    
    int bytes_sent = -1;
    while(total_sent < bytes_sent) {
        bytes_sent = send(socket_fd, message + total_sent, bytes_left, 0);
        if(bytes_sent == -1) {
            perror("Send failed");
            return -1;
        }

        total_sent += bytes_sent;
        bytes_left -= bytes_sent;
    }

    return total_sent;
}

int main() {
    return 0;
}
