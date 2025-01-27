//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#include "Socket.h"
#include <iostream>

void Socket::initialize() {} // Posix does not need initialize

ssize_t Socket::sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) {
    return ::sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

ssize_t Socket::recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
    return ::recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

int Socket::close(int sockfd) {
    return ::close(sockfd);
}

void Socket::cleanup() {} // Posix does not need cleanup