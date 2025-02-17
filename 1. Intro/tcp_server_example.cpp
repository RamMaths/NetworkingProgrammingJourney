#include <iostream>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  if(serverSocket < 0) {
    std::cerr << "Socket creation failed" << std::endl;
    return -1;
  }

  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(8080);

  if(bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    std::cerr << "Bind failed" << std::endl;
    return -1;
  }

  if(listen(serverSocket, 3) < 0) {
    std::cerr << "Listen failed" << std::endl;
    return -1;
  }

  std::cout << "Server listening for connections..." << std::endl;

  int clientSocket;

  sockaddr_in clientAddress;
  socklen_t clientLength = sizeof(clientAddress);
  clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientLength);

  if(clientSocket < 0) {
    std::cerr << "Client accept failed" << std::endl;
    return -1;
  }

  char buffer[1024] = {0};
  read(clientSocket, buffer, 1024);
  std::cout << "Message received: " << buffer << std::endl;

  const char* response = "Hello from server";
  send(clientSocket, response, strlen(response), 0);

  close(clientSocket);
  close(serverSocket);

  return 0;
}
