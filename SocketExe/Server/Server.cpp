//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//
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
    socklen_t len = sizeof(cliaddr);

    memset(&cliaddr, 0, sizeof(cliaddr));

    std::cout << "Waiting for message..." << std::endl;
    n = Socket::recvFrom(sockfd, (char *)buffer, MAXLINE, 0,(struct sockaddr *)&cliaddr, &len);
    if (n < 0) {
        Socket::close(sockfd);
        Socket::cleanup();
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    std::cout << "Message received from the client: " << buffer << std::endl;

    Socket::sendTo(
      sockfd,
      (const char *)buffer,
      strlen(buffer),
      0,
      (const struct sockaddr *)&cliaddr,
      len);

    std::cout << "Message sent to the client." << std::endl;

    Socket::close(sockfd);
    Socket::cleanup();
    return 1;
}
