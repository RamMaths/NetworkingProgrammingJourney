#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
int main() {
  // Create a socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    std::cerr << "Socket creation error" << std::endl;
    return -1; 
  }

  // Define server address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);

  // Convert IP address
  if (inet_pton(AF_INET, "192.168.1.1", &serverAddress.sin_addr) <= 0) {
    std::cerr << "Invalid address or Address not supported" << std::endl;
    return -1; 
  }
  // Connect to server
  if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    std::cerr << "Connection failed" << std::endl;
    return -1; 
  }

  // Send data to server
  const char *message = "Hello Server";
  send(clientSocket, message, strlen(message), 0);
  close(clientSocket);

  return 0;
}
