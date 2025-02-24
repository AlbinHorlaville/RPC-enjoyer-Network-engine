//
// Created by Albin Horlaville on 10/02/2025.
//

#include "Client.h"
#include "FormatPackage.h"
#include "Socket.h"
#include <array>
#include <span>
#include <algorithm>

#define PORT_SEND 5555
#define PORT_RECEIVE 5556

constexpr uint64_t RELIABILITY_MASK = static_cast<uint64_t>(1) << 62;


Client::Client() {
  connected = false;
  ping_id = 0;
  uuid = 0;
  token = 0;
  msg_id = 0;
  connectionSockfd = 0;
  ip_server = "";
  version = "1.0.0";
  latence = 0;
  timerDisconnect = std::make_unique<Timer>();
  timerPing = std::make_unique<Timer>();
  timerReconnect = std::make_unique<Timer>();
  timerStopReconnect = std::make_unique<Timer>();
  for (int i = 0; i < received_packages.size(); i++) {
    received_packages[i] = true;
  }
  last_rcv_id = 0;
}

void Client::Ping(){
  struct Ping package{};
  package.ping_id = ping_id;
  ping_id += 1;
  package.ping_timer = getTimestamp();
  package.uuid = uuid;
  package.size = sizeof(package);
  std::vector<char> Data = package.serialize();
  stream->SendData(Data);
  //std::cout << "Ping " << package.ping_id << std::endl;
}

void Client::ConnectTo(const std::string& ip){
  Socket::initialize();
  connectionSockfd = Socket::connect(ip, PORT_RECEIVE);

  // Send the package CONNECT
  struct Connect package;
  package.version = version;
  package.size = sizeof(package);
  std::vector<char> Data = package.serialize();
  ip_server = ip;
  Socket::sendTo(connectionSockfd, ip, PORT_SEND, Data);

  std::array<char, 65535> buffer;

  std::string ip_client;

  // Wait until we receive a CONNECT or a RECONNECT package
  long long read_bytes = Socket::recvFrom(connectionSockfd, ip_client, std::span<char, 65535>(buffer));
  if (read_bytes < 0) {
    Socket::close(connectionSockfd);
    Socket::cleanup();
    exit(EXIT_FAILURE);
  }

  // Deserialize Package
  struct Connect_ACK connect_ack{};
  connect_ack.deserialize(buffer);
  uuid = connect_ack.uuid;
  token = connect_ack.token;

  // Get the reliability
  bool reliability = connect_ack.stream_id & RELIABILITY_MASK;

  std::cout << "Reliability : " << reliability << std::endl;

  // Create Stream
  stream = std::make_unique<Stream>(connect_ack.stream_id, reliability, ip, connect_ack.port, connect_ack.port+1);

  // Start the PING timer
  timerPing->setInterval([this](){Ping();}, 200);

  // Start the DISCONNECT timer
  timerDisconnect->setTimeout([this](){OnDisconnect();}, 1000);

  connected = true;
}

void Client::SendData(std::string const& data) {
  // Send DATA package to the server
  struct Data package;
  package.uuid = uuid;
  package.stream_id = stream->getId();
  package.msg_id = msg_id++;
  package.data = data;
  package.size = sizeof(package);
  std::vector<char> Data = package.serialize();
  stream->SendData(Data);

  if (stream->isReliable()) {
    // Add this package, starting him waiting it ack
    packages_waiting_ack[package.msg_id] = package;
  }
}

void Client::CloseConnexion() {
  connected = false;
  // Fermeture de la connexion
  if (timerPing) {
    timerPing->stop();
  }
  if (timerReconnect) {
    timerReconnect->stop();
  }
  stream->Close();
  stream.reset();
}

void Client::Disconnect() {
  // Send the DISCONNECT package to the server.
  struct Disconnect package;
  package.uuid = uuid;
  package.size = sizeof(package);
  std::vector<char> Data = package.serialize();
  stream->SendData(Data);
  CloseConnexion();
}

void Client::Reconnect() {
  if (connected) return;

  // Try to RECONNECT to the server.
  struct Reconnect package;
  package.uuid = uuid;
  package.token = token;
  package.size = sizeof(package);
  std::vector<char> Data = package.serialize();
  Socket::sendTo(connectionSockfd, ip_server, PORT_SEND, Data);

  std::array<char, 65535> buffer;

  std::string ip_client;

  // Wait until we receive a CONNECT or a RECONNECT package
  long long read_bytes = Socket::recvFrom(connectionSockfd, ip_client, std::span<char, 65535>(buffer));
  if (read_bytes < 0) {
    Socket::close(connectionSockfd);
    Socket::cleanup();
    exit(EXIT_FAILURE);
  }

  // Deserialize Package
  struct Connect_ACK connect_ack{};
  connect_ack.deserialize(buffer);
  uuid = connect_ack.uuid;
  token = connect_ack.token;

  // Create Stream
  stream = std::make_unique<Stream>(1, false, ip_server, connect_ack.port, connect_ack.port+1);

  // Start the PING timer
  timerPing->setInterval([this](){Ping();}, 200);

  // Start the DISCONNECT timer
  timerDisconnect->setTimeout([this](){OnDisconnect();}, 1000);

  connected = true;

  std::cout << "Reconnect !" << std::endl;
}

uint16_t Client::GetLatence() const {
  return latence;
}

void Client::OnDisconnect() {
  std::cout << "OnDisconnect !" << std::endl;
  stream->Close();
  timerPing->stop();
  timerDisconnect->stop();
  timerReconnect->setInterval([this](){Reconnect();}, 1000);
  timerStopReconnect->setTimeout([this](){CloseConnexion();}, 5000);
}

int Client::ReceiveData() {
  std::array<char, 65535> bufferReceive;
  std::vector<char> bufferSend;
  stream->ReceiveData(std::span<char, 65535>(bufferReceive));

  int res = -1;

  switch (bufferReceive[0]) {
    case PING: {
      // PONG
      // Deserialize Package
      struct Ping pong{};
      pong.deserialize(bufferReceive);

      // Calcul the latence
      latence = getTimestamp() - pong.ping_timer;

      // Reset the timer
      timerDisconnect->stop();
      timerDisconnect->setTimeout([this](){OnDisconnect();}, 1000);
      //std::cout << "Pong " << pong.ping_id << std::endl;
      res =  PING;
      break;
    }
    case DATA: {
      // DATA
      // Deserialize Package
      struct Data data;
      data.deserialize(bufferReceive);

      // Print Message
      std::cout << "Message receive from Server : " << data.data << std::endl;
      std::cout << "Sending back ACK..." << std::endl;

      // Send DATA_ACK
      struct Data_ACK data_ack{};
      data_ack.uuid = uuid;
      data_ack.msg_id = data.msg_id;

      if (stream->isReliable()) {
        // Reliability bitset
        if (data.msg_id > last_rcv_id) {
          int diff = static_cast<int>(data.msg_id - last_rcv_id);
          int size = static_cast<int>(received_packages.size());
          received_packages <<= static_cast<size_t>(std::min(diff, size));
          received_packages[0] = true;
          last_rcv_id = data.msg_id;
        }else {
          received_packages[last_rcv_id - data.msg_id] = true;
        }

        data_ack.previous_packages = received_packages;
      }

      data_ack.size = sizeof(data_ack);
      bufferSend = data_ack.serialize();
      stream->SendData(bufferSend);

      res = DATA;
      break;
    }
    case DATA_ACK: {
      // DATA ACK
      // Deserialize Package
      struct Data_ACK ack{};
      ack.deserialize(bufferReceive);
      std::cout << "DATA_ACK received : " << ack.msg_id << std::endl;

      std::cout << "BITSET : ";
      for (int i=0; i < 30; i++) {
        std::cout << ack.previous_packages[i];
      }
      std::cout << std::endl;

      // Reliability
      if (stream->isReliable()) {
        if (packages_waiting_ack.contains(ack.msg_id)) {
          packages_waiting_ack.erase(ack.msg_id);
        }
        for (int i = 0; i < ack.previous_packages.size(); i++) {
          if (packages_waiting_ack.contains(ack.msg_id - i)) {
            Data package = packages_waiting_ack[ack.msg_id - i];
            std::vector<char> data = package.serialize();
            stream->SendData(data);
          }
        }
      }

      res = DATA_ACK;
      break;
    }
    default: {
      // Never happen
      break;
    }
  }
  bufferReceive.fill(0);
  return res;
}
