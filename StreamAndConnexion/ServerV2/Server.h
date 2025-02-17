//
// Created by Albin Horlaville & Hugo Girard on 10/02/2025.
//

#ifndef SERVER_H
#define SERVER_H

#include "Timer.h"
#include <cstdint>
#include <string>
#include <span>
#include <map>

class Stream;

struct ClientInfo {
    std::string ip;
    uint16_t port;
    uint64_t token;
    uint64_t uuid;
    Stream* stream;
    Timer* timer;
};

class Server {
    std::map<uint64_t, ClientInfo> clients;
    int connectionSockfd;
    uint64_t msg_id;
public:
    Server();
    ~Server();

    void Listen();
    void OnClientConnected(std::span<char, 65535> buffer, const std::string& ip_client);
    void Receive();
    void Pong(uint64_t uuid, uint64_t ping_id);
    void OnClientDisconnected(uint64_t uuid);
    void SendData(uint64_t const& uuid, std::string const& data);
};



#endif //SERVER_H
