//
// Created by Albin Horlaville on 10/02/2025.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <cstdint>
#include <functional>
#include <memory>

class Stream;
class Timer;

class Client {
    // un timer qui envoie des pings toutes les 200ms
    // un timer qui call OnDisconnect en arrivant à 0s sauf s'il est reset avan
private:
    std::unique_ptr<Stream> stream;
    std::unique_ptr<Timer> timerPing;
    std::unique_ptr<Timer> timerDisconnect;
    std::unique_ptr<Timer> timerReconnect;
    std::unique_ptr<Timer> timerStopReconnect;
    uint64_t ping_id;
    uint64_t uuid;
    uint64_t token;
    uint64_t msg_id;
    int sockfd;
    std::string ip_server;
    uint16_t port_server;
    std::string version;
    uint16_t latence;
public:
    Client();
    void ConnectTo(const std::string& ip);
    //void OnConnectionEvent(std::function<void(bool, uint64_t)> handler); //Here the bool represent the success of the connection
    void SendData(std::string const& text);
    void ReceiveData();
    void Disconnect();
    uint16_t GetLatence() const;

private:
    void OnDisconnect();
    void Reconnect();
    void CloseConnexion();
    void Ping();
    void CreateStream(bool reliable);
};



#endif //CLIENT_H
