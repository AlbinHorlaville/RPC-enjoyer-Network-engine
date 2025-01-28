//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#include "Socket.h"
#include <iostream>

void Socket::initialize() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        exit(1);
    }
}

ssize_t Socket::sendTo(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) {
    return ::sendto(sockfd, (const char*)buf, (int)len, flags, dest_addr, (int)addrlen);
}

ssize_t Socket::recvFrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
    int received = ::recvfrom(sockfd, (char *)buf, (int)len, flags, src_addr, (int *)addrlen);
    if (received == SOCKET_ERROR) {
        int error_code = WSAGetLastError();
        std::cerr << "recvfrom failed. Error code: " << error_code << std::endl;
        return -1; // Explicit failure
    }
    return received;
}

int Socket::close(int fd) {
    return closesocket(fd);
}

void Socket::cleanup() {
    WSACleanup();
}