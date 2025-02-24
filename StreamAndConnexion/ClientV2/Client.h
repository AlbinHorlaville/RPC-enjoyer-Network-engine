//
// Created by Albin Horlaville & Hugo Girard on 10/02/2025.
//

#ifndef CLIENT_H
#define CLIENT_H

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <bitset>
#include "FormatPackage.h"
#include "Timer.h"
#include "Stream.h"

struct history {
    Data data;
    bool acknowledged;
};

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
    // RELIABILITY
    std::map<uint64_t, Data> packages_waiting_ack; // A stocke les paquets dont il n'a pas encore reçu l'ack
    std::bitset<30> received_packages; // B stocke la listes des status (reçu / non reçu) des 30 derniers paquets.
    // A envoie un paquet Data à B.
    // A stocke dans une map les paquets en attente d'ack associés à leur id.
    // B stocke la listes des status (reçu / non reçu) des 30 derniers paquets.
    // Quand A envoie un paquet DATA, il le stock dans la map.
    // Quand B reçoit le paquet, il met à jour son bitset puis l'envoie dans l'ack à A.
    // Quand A reçoit un ack, il supprime de la map les paquets qui ont été acquitté et renvoie ceux qui ne le sont pas.
    uint64_t last_rcv_id;
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
