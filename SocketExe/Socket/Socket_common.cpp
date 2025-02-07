//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#include <fmt/core.h>
#include "Socket.h"

int Socket::socket(int domain) {
    return ::socket(domain, SOCK_DGRAM, IPPROTO_UDP);
}

int Socket::bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    return ::bind(sockfd, addr, addrlen);
}

int Socket::listen(const std::string& endpoint, uint16_t port)
{
    sockaddr local_endpoint = StringToIp(endpoint, port);
    int sockfd = socket(local_endpoint.sa_family);
    if (int error = bind(sockfd, &local_endpoint, sizeof(local_endpoint)); error != 0)
    {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int Socket::connect(const std::string& serverIp, uint16_t port)
{
    sockaddr local_endpoint = StringToIp(serverIp, port);
    int sockfd = socket(local_endpoint.sa_family);
    if (int error = bind(sockfd, &local_endpoint, sizeof(local_endpoint)); error != 0)
    {
        close(sockfd);
        return -1;
    }

    return sockfd;
}
