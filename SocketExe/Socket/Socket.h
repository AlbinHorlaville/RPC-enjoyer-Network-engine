//
// Created by Albin Horlaville and Hugo Girard on 27/01/2025.
//

#ifndef SOCKET_H
#define SOCKET_H

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

      // Creates an unbound socket in the specified domain.
      // Return socket file descriptor.
      static int socket(int domain, int type, int protocol);

      // Assigns address to the unbound socket.
      static int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

      // Send a message on the socket.
      static ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);

      // Receive a message from the socket.
      static ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);

      // Close a file descriptor.
      static int close(int fd);

      // Clean the library
      static void cleanup();
};

#endif //SOCKET_H
