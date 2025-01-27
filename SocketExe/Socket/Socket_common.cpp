//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#include "Socket.h"

int Socket::socket(int domain, int type, int protocol) {
    return ::socket(domain, type, protocol);
}

int Socket::bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    return ::bind(sockfd, addr, addrlen);
}
