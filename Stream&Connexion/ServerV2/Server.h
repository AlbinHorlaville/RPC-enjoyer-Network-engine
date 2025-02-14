//
// Created by Albin Horlaville on 10/02/2025.
//

#ifndef SERVER_H
#define SERVER_H

#include <cstdint>
#include <functional>

class Stream;

class Server {
    // Qu'est ce qu'on met en variable dans le serveur, la liste des clients ??
public:
    Server();
    ~Server();

    void Listen(uint16_t port);

    void OnClientConnected(std::function<void(uint64_t)> handler);
    void Receive(std::function<void(uint64_t)> handler);
    void Pong();
    void OnClientDisconnected(std::function<void(uint64_t)> handler);

    std::unique_ptr<Stream> CreateStream(uint64_t client, bool reliable);

    void CloseStream(const Stream& stream);
};



#endif //SERVER_H
