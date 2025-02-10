//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <array>

#include "Socket.h"

#define PORT_SEND 5555
#define PORT_RECEIVE 5556
#define MAXLINE 1024

int main(int argc, char *argv[]){
  Socket::initialize();

  int sockfd;
  char addr[20];
  char bufferSend[MAXLINE];
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

  if ((sockfd = Socket::connect(addr, PORT_RECEIVE)) < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }


  socklen_t len = sizeof(cliaddr);

  std::span data(bufferSend, std::strlen(bufferSend));
  Socket::sendTo(sockfd, addr, PORT_SEND, data);

  std::cout << "Message sent to the server: " << bufferSend << std::endl;
  std::cout << "Waiting for an answer..." << std::endl;

  std::string from_ip;
  from_ip.resize(255);
  std::array<char, 65535> bufferReceive;
  long long read_bytes;
  read_bytes = Socket::recvFrom(sockfd, from_ip, bufferReceive);
  if (read_bytes < 0) {
    Socket::close(sockfd);
    Socket::cleanup();
    exit(EXIT_FAILURE);
  }

  bufferReceive[read_bytes] = '\0';
  std::cout << "Message received from the server: " << bufferReceive.data() << std::endl;

  Socket::close(sockfd);
  Socket::cleanup();

  return strcmp(bufferSend, bufferReceive.data()) == 0;
  }
