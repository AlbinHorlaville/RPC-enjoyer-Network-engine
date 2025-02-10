//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <span>
#include <cstdint>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

class Socket { // using UDP
public:
      Socket() = delete;
      Socket(const Socket&) = delete;

      // Platform-independent socket initialization
      static void initialize();

      // Bind the socket to an adress
      // Return socket file descriptor.
      static int listen(const std::string& endpoint, uint16_t port);

      // Bind the socket to an adress
      // Return socket file descriptor.
      static int connect(const std::string& serverIp, uint16_t port);

      // Send a message on the socket.
      static long sendTo(int sockfd, const std::string &to, uint16_t port, std::span<const char> message);

      // Receive a message from the socket.
      static long recvFrom(int sockfd, std::string &from, std::span<char, 65535> message);

      // Close a file descriptor.
      static int close(int fd);

      // Clean the library
      static void cleanup();

private:
      // Creates an unbound socket in the specified domain.
      // Fill AF_INET for IPv4 or AF_INET6 for IPv6 as argument.
      // Return socket file descriptor.
      static int socket(int domain);

      // Assigns address to the unbound socket.
      static int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

      static std::string IpToString(const sockaddr* sa);

      static sockaddr StringToIp(const std::string& ip, uint16_t port);
};

#endif //SOCKET_H
