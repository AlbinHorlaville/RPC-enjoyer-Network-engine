//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#include "Socket.h"
#include <iostream>

void Socket::initialize() {} // Posix does not need initialize

long Socket::sendTo(const int sockfd, const std::string &to, uint16_t port, std::span<const char> message) {
    const sockaddr destination = StringToIp(to, port);
    ssize_t error = sendto(sockfd,
        message.data(),
        message.size(),
        0,
        &destination,
        sizeof(destination));
    return error;
}

long Socket::recvFrom(int sockfd, std::string &from, std::span<char, 65535> message) {
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
    const ssize_t read_bytes = recvfrom(sockfd,
        message.data(),
        message.size_bytes(),
        0,
        reinterpret_cast<sockaddr*>(&peer_addr),
        &peer_addr_len);

    from = IpToString(reinterpret_cast<const sockaddr*>(&peer_addr));

    return read_bytes;
}

int Socket::close(int sockfd) {
    return ::close(sockfd);
}

void Socket::cleanup() {} // Posix does not need cleanup