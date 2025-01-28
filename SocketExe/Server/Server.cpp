//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//
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
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = Socket::socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket with the server address
    if (Socket::bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
      perror("Bind failed");
      exit(EXIT_FAILURE);
    }

    int n;
    socklen_t len;

    len = sizeof(cliaddr); // len is value/result

    n = Socket::recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,(struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    std::cout << "Client : " << buffer << std::endl;

    Socket::sendto(
      sockfd,
      (const char *)buffer,
      strlen(buffer),
      SD_SEND,
      (const struct sockaddr *)&cliaddr,
      sizeof(servaddr));

    std::cout << "Message sent to the client." << std::endl;

    Socket::close(sockfd);
    Socket::cleanup();
    return 0;
}
