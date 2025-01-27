//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#include <iostream>

#include "Socket.h"

#define PORT 5555
#define MAXLINE 1024

int main(int argc, char *argv[]){
  Socket::initialize();

  int sockfd;
  char addr[20];
  char bufferSend[MAXLINE];
  char bufferReceive[MAXLINE];
  struct sockaddr_in servaddr;

  std::cout << "Enter Server IP Address: ";
  std::cin >> addr;
  std::cout << "Enter message to send: ";
  std::cin >> bufferSend;

  if ((sockfd = Socket::socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr(addr);

  int n;
  socklen_t len;

  Socket::sendto(
    sockfd,
    (const char *)bufferSend,
    strlen(bufferSend),
    MSG_SEND,
    (const struct sockaddr *)&servaddr,
    sizeof(servaddr));

  std::cout << "Message sent to the server." << std::endl;

  n = Socket::recvfrom(sockfd, (char *)bufferReceive, MAXLINE, MSG_WAITALL,(struct sockaddr *)&servaddr, &len);
  bufferReceive[n] = '\0';
  std::cout << "Server : " << bufferReceive << std::endl;

  Socket::close(sockfd);
  Socket::cleanup();
  return 0;
  }
