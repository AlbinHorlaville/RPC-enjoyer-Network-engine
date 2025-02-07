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
    sockaddr result {};
    int error = inet_pton(AF_INET, ip.c_str(), &result);
    if (error == 1) {
        result.sa_family = AF_INET;
        reinterpret_cast<sockaddr_in*>(&result)->sin_port = htons(port);
        return result;
    }

    memset(&result, 0, sizeof(result));
    error = inet_pton(AF_INET6, ip.c_str(), &result);
    if (error == 1) {
        result.sa_family = AF_INET6;
        reinterpret_cast<sockaddr_in6*>(&result)->sin6_port = htons(port);
        return result;
    }
    memset(&result, 0, sizeof(result));
    return result;
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
