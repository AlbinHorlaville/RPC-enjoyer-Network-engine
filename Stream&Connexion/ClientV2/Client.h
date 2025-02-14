//
// Created by Albin Horlaville on 10/02/2025.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <cstdint>
#include <functional>

class Stream;

class Client {
    // un timer qui envoie des pings toutes les 200ms
    // un timer qui call OnDisconnect en arrivant à 0s sauf s'il est reset avant
  private:
    Stream *stream;
public:
    Client();

    void ConnectTo(const std::string& ip, uint16_t port);
    void OnConnectionEvent(std::function<void(bool, uint64_t)> handler); //Here the bool represent the success of the connection
    void OnDisconnect(std::function<void> handler);
    void Ping() const;
    std::unique_ptr<Stream> CreateStream(bool reliable);
    std::unique_ptr<Stream> OnCreateStream(std::function<void(bool, uint64_t)> handler);
};



#endif //CLIENT_H
