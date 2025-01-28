//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#include <iostream>
#include <cstring>
#include <cstdlib>

#include "Socket.h"

#define PORT 5555
#define MAXLINE 1024

int main(int argc, char *argv[]){
  Socket::initialize();

  int sockfd;
  char addr[20];
  char bufferSend[MAXLINE];
  char bufferReceive[MAXLINE];
  struct sockaddr_in servaddr, cliaddr;

  if (argc == 3) {
    strcpy(addr, argv[1]); // IP adress
    strcpy(bufferSend, argv[2]); // Message
  }else {
    std::cout << "Enter Server IP Address: ";
    std::cin >> addr;
    std::cout << "Enter message to send: ";
    std::cin >> bufferSend;
  }

  if ((sockfd = Socket::socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr(addr);

  int n;
  socklen_t len = sizeof(cliaddr);

  Socket::sendTo(
    sockfd,
    (const char *)bufferSend,
    strlen(bufferSend),
    0,
    (const struct sockaddr *)&servaddr,
    sizeof(servaddr));

  std::cout << "Message sent to the server: " << bufferSend << std::endl;

  n = Socket::recvFrom(sockfd, (char *)bufferReceive, MAXLINE, 0,(struct sockaddr *)&cliaddr, &len);
  if (n < 0) {
    Socket::close(sockfd);
    Socket::cleanup();
    exit(EXIT_FAILURE);
  }

  bufferReceive[n] = '\0';
  std::cout << "Message received from the server: " << bufferReceive << std::endl;

  Socket::close(sockfd);
  Socket::cleanup();

  if (strcmp(bufferSend, bufferReceive) == 0) {
    return 1;
  }
  return 0;
  }
