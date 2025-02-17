//
// Created by Albin Horlaville on 10/02/2025.
//

#include "Server.h"
#include "Stream.h"
#include "Socket.h"
#include "FormatPackage.h"
#include <random>

#define PORT_SEND 5556
#define PORT_RECEIVE 5555

uint64_t generateRandomUint64() {
    // Initialize random number generator with a random seed
    std::random_device rd;
    std::mt19937_64 gen(rd());  // 64-bit Mersenne Twister engine

    // Define a uniform distribution for uint64_t (in the full range)
    std::uniform_int_distribution<uint64_t> dis;

    // Generate and return a random uint64_t
    return dis(gen);
}

uint16_t generateRandomPort() {
    std::random_device rd; // Obtain a random seed
    std::mt19937 gen(rd());  // Mersenne Twister random number generator
    std::uniform_int_distribution<uint16_t> dis(1024, 60000);  // Uniform distribution between 1024 and 60000

    return dis(gen);  // Generate and return a random port number
}

Server::Server() : connectionSockfd(0), msg_id(0){}

Server::~Server() {}

void Server::getIPandPort(std::string* ip, uint16_t* port) {
    std::string ip_copy = *ip;
    auto pos = ip->find_last_of (':');
    if (pos != std::string::npos) {
        *ip = ip_copy.substr (0,pos);
        std::string port_str = ip_copy.substr (++pos);
        *port = atoi(port_str.c_str());
    }
}

void Server::Listen() {
    Socket::initialize();
    if (connectionSockfd == 0) {
        connectionSockfd = Socket::listen("127.0.0.1", PORT_RECEIVE);
        if (connectionSockfd < 0) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }
    }

    std::array<char, 65535> buffer;

    std::string ip_client;

    // Wait until we receive a CONNECT or a RECONNECT package
    long long read_bytes = Socket::recvFrom(connectionSockfd, ip_client, std::span<char, 65535>(buffer));
    if (read_bytes < 0) {
        Socket::close(connectionSockfd);
        Socket::cleanup();
        exit(EXIT_FAILURE);
    }

    OnClientConnected(buffer, ip_client);
}

uint64_t Server::GetTestClientID() const {
    return clients.begin()->second.uuid;
}

void Server::OnClientConnected(std::span<char, 65535> buffer, const std::string& ip_client) {
    if (buffer.empty()) {
        throw std::invalid_argument("Buffer is empty");
    }

    // Separate ip and port
    std::string ip = ip_client;
    uint16_t port = 0;
    getIPandPort(&ip, &port);

    // CONNECT
    if (buffer[0] == 1) {
        const uint64_t newToken = generateRandomUint64();
        const uint16_t newPort = generateRandomPort();
        const uint64_t uuid = clients.size();
        Connect_ACK connect_ack{sizeof(connect_ack), newPort, uuid, newToken};

        Socket::sendTo(connectionSockfd, ip, PORT_SEND, connect_ack.serialize());
        ClientInfo client{ip, newPort, newToken, uuid, nullptr, new Timer()};
        auto* stream = new Stream(1, false, ip, newPort+1, newPort);
        client.stream = stream;
        clients[uuid] = client;
        std::cout << "Client port : " << client.port << std::endl;
    // RECONNECT
    } else if (buffer[0] == 2) {
        struct Reconnect reconnect{};
        reconnect.deserialize(buffer);
        if(clients.contains(reconnect.uuid) && reconnect.token == clients[reconnect.uuid].token) {
            auto* stream = new Stream(1, false, ip, clients[reconnect.uuid].port+1, clients[reconnect.uuid].port);
            clients[reconnect.uuid].stream = stream;
            clients[reconnect.uuid].timer = new Timer();
            Connect_ACK connect_ack{sizeof(connect_ack), clients[reconnect.uuid].port, reconnect.uuid, reconnect.token};
            Socket::sendTo(connectionSockfd, ip, PORT_SEND, connect_ack.serialize());
        }
    }
}

void Server::Receive() {
    std::array<char, 65535> bufferReceive;
    std::vector<char> bufferSend;
    clients.begin()->second.stream->ReceiveData(std::span<char, 65535>(bufferReceive));

    switch (bufferReceive[0]) {
        case 3: // DISCONNECT
        {
            struct Disconnect disconnect{};
            disconnect.deserialize(bufferReceive);
            OnClientDisconnected(disconnect.uuid);
            Listen();
            break;
        }
        case 5: // PING
        {
            // Deserialize Package
            struct Ping ping{};
            ping.deserialize(bufferReceive);
            ClientInfo pongClient = clients[ping.uuid];
            pongClient.timer->stop();
            auto function = [this, ping] () { OnClientDisconnected(ping.uuid);};
            pongClient.timer->setTimeout(function,1000);
            std::cout << "Pong : " << ping.ping_id << std::endl;
            Pong(ping.uuid, ping.ping_id);
            break;
        }
        case 6: // DATA
        {
            // Deserialize Package
            struct Data data;
            data.deserialize(bufferReceive);

            // Print Message
            std::cout << "Message receive from Client : " << data.data << std::endl;
            std::cout << "Sending back ACK..." << std::endl;

            // Send DATA_ACK
            struct Data_ACK data_ack{};
            data_ack.uuid = data.uuid;
            data_ack.last_rcv_id = data.msg_id;
            data_ack.last_rcv = 0;
            data_ack.size = sizeof(data_ack);
            bufferSend = data_ack.serialize();
            clients[data.uuid].stream->SendData(bufferSend);
            break;
        }
        case 7: // DATA ACK
        {
            // Deserialize Package
            struct Data_ACK ack{};
            ack.deserialize(bufferReceive);
            std::cout << "DATA_ACK received : " << ack.last_rcv_id << std::endl;
            // A finir : Faire un suivi des messages envoyer pour que l'ack ait un intérêt.
            break;
        }
        default:
            // Never happen
            break;
    }
    bufferReceive.fill(0);
}

void Server::SendData(uint64_t const& uuid, std::string const& data) {
    // Send DATA package to the server
    struct Data package;
    Stream* clientStream = clients[uuid].stream;
    package.uuid = uuid;
    package.stream_id = clientStream->getId();
    package.flag = Flag::NONE;
    package.msg_id = msg_id++;
    package.data = data;
    package.size = sizeof(package);
    std::vector<char> Data = package.serialize();
    clientStream->SendData(Data);
}

void Server::Pong(const uint64_t uuid, const uint64_t ping_id) {
    Ping ping{sizeof(ping), uuid, ping_id};
    clients[uuid].stream->SendData(ping.serialize());
}

void Server::OnClientDisconnected(const uint64_t uuid) {
    clients[uuid].stream->Close();
    clients[uuid].timer->stop();
    std::cout << "Client disconnected : " << clients[uuid].uuid << std::endl;
}

void Server::CreateStream(const uint64_t uuid, const bool reliable) {
    auto* stream = new Stream(1, reliable, clients[uuid].ip, PORT_RECEIVE, clients[uuid].port);
    clients[uuid].stream = stream;
}
