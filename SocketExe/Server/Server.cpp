//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//
//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <array>

#include "Socket.h"

#define PORT_SEND 5556
#define PORT_RECEIVE 5555
#define MAXLINE 1024

int main(int argc, char *argv[]){
    Socket::initialize();

    int sockfd;
    char addr[20];
    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = Socket::listen("127.0.0.1", PORT_RECEIVE)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }


    socklen_t len = sizeof(cliaddr);

    memset(&cliaddr, 0, sizeof(cliaddr));

    std::cout << "Waiting for message..." << std::endl;
    std::string from_ip;
    from_ip.resize(255);
    std::array<char, 65535> buffer;
    long long read_bytes;
    read_bytes = Socket::recvFrom(sockfd, from_ip, buffer);
    if (read_bytes < 0) {
        Socket::close(sockfd);
        Socket::cleanup();
        exit(EXIT_FAILURE);
    }

    buffer[read_bytes] = '\0';
    std::cout << "Message received from the client: " << buffer.data() << std::endl;

    uint16_t port = 0;
    auto pos = from_ip.find_last_of (':');
    std::string ip;
    if (pos != std::string::npos) {
        ip = from_ip.substr (0,pos);
        std::string port_str = from_ip.substr (++pos);
        port = atoi(port_str.c_str());
    }
    Socket::sendTo(sockfd, ip, port, std::span {buffer.data(), static_cast<unsigned long>(read_bytes)});

    std::cout << "Message sent to the client." << std::endl;

    Socket::close(sockfd);
    Socket::cleanup();
    return 1;
}
