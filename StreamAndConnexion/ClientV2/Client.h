//
// Created by Albin Horlaville & Hugo Girard on 10/02/2025.
//

#ifndef CLIENT_H
#define CLIENT_H

#pragma once

#include <cstdint>
#include <functional>
#include "Timer.h"
#include "Stream.h"

class Client {
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
    int connectionSockfd;
    std::string ip_server;
    std::string version;
    uint16_t latence;
    bool connected;
public:
    Client();
    void ConnectTo(const std::string& ip);
    //void OnConnectionEvent(std::function<void(bool, uint64_t)> handler); //Here the bool represent the success of the connection
    void SendData(std::string const& text);
    int ReceiveData();
    void Disconnect();
    uint16_t GetLatence() const;
    void Reconnect();

private:
    void OnDisconnect();

    void CloseConnexion();
    void Ping();
};



#endif //CLIENT_H
