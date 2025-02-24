//
// Created by Albin Horlaville & Hugo Girard on 10/02/2025.
//

#ifndef SERVER_H
#define SERVER_H

#pragma once

#include "Timer.h"
#include "FormatPackage.h"
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
public:
    std::map<uint64_t, ClientInfo> clients;
private:
    int connectionSockfd;
    uint64_t msg_id;
    uint64_t last_stream_id;
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
    Server();
    ~Server();

    void Listen();
    void OnClientConnected(std::span<char, 65535> buffer, const std::string& ip_client);
    int Receive();
    void Pong(uint64_t uuid, uint64_t ping_id);
    void OnClientDisconnected(uint64_t uuid);
    void SendData(uint64_t const& uuid, std::string const& data);
};



#endif //SERVER_H
