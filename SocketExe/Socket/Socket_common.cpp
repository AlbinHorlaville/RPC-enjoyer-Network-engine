//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#include <fmt/core.h>
#include "Socket.h"

std::string Socket::IpToString(const sockaddr* sa)
{
    switch(sa->sa_family)
    {
        case AF_INET: {
            char ip[INET_ADDRSTRLEN + 6];
            const char* ret = inet_ntop(AF_INET,
                &reinterpret_cast<const sockaddr_in*>(sa)->sin_addr,
                ip,
                INET_ADDRSTRLEN);
            return fmt::format("{}:{}", ret, ntohs(reinterpret_cast<const sockaddr_in*>(sa)->sin_port));
        }
        case AF_INET6: {
            char ip[INET6_ADDRSTRLEN + 8];
            const char* ret = inet_ntop(AF_INET6,
                &reinterpret_cast<const sockaddr_in6*>(sa)->sin6_addr,
                ip+ 1,
                INET6_ADDRSTRLEN);
            return fmt::format("[{}]:{}", ret, ntohs(reinterpret_cast<const sockaddr_in6*>(sa)->sin6_port));
        }
    }

    return "";
}

sockaddr Socket::StringToIp(const std::string& ip, uint16_t port)
{
    sockaddr_in result{};  // Use sockaddr_in directly
    if (inet_pton(AF_INET, ip.c_str(), &result.sin_addr) == 1) {
        result.sin_family = AF_INET;
        result.sin_port = htons(port);
        return *reinterpret_cast<sockaddr*>(&result);  // Return as sockaddr
    }

    sockaddr_in6 result6{};  // Try IPv6 if IPv4 fails
    if (inet_pton(AF_INET6, ip.c_str(), &result6.sin6_addr) == 1) {
        result6.sin6_family = AF_INET6;
        result6.sin6_port = htons(port);
        return *reinterpret_cast<sockaddr*>(&result6);  // Return as sockaddr
    }

    std::cerr << "Invalid IP address: " << ip << std::endl;
    sockaddr empty{};
    return empty;  // Return an empty sockaddr on failure
}

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
