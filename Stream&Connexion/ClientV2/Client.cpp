//
// Created by Albin Horlaville on 10/02/2025.
//

#include "Client.h"
#include "FormatPackage.h"
#include "Timer.h"
#include "Stream.h"
#include "Socket.h"
#include "fmt/base.h"

Client::Client() : stream(nullptr), timerPing(nullptr), timerDisconnect(nullptr), ping_id(0), uuid(-1), sockfd(0){}

void Client::ConnectTo(const std::string& ip, uint16_t port){
  Socket::initialize();
  sockfd = Socket::connect(ip, port);

  // Send the package CONNECT
  struct Connect package;
  package.version = version;
  package.size = sizeof(Connect);
  std::vector<char> Data = package.serialize();
  stream->SendData(Data);

  // Start the PING timer
  timerPing = new Timer();
  timerPing->setInterval(Ping, 200);

  // Start the DISCONNECT timer
  timerDisconnect = new Timer();
  timerDisconnect->setTimeout(OnDisconnect, 1000);
}

void Client::SendData(std::string const& data) {
  // Send DATA package to the server
  struct Data package;
  package.uuid = uuid;
  package.stream_id = stream->getId();
  package.flag = Flag::NONE;
  package.msg_id = msg_id++;
  package.data = data;
  package.size = sizeof(package);
  std::vector<char> Data = package.serialize();
  stream->SendData(Data);
}

void Client::ReceiveData() {
  std::vector<char> buffer;
  stream->ReceiveData(std::span<char, 65535>(buffer));

  switch (buffer[0]) {
    case ""
  }
}

void Client::OnConnectionEvent(std::function<void(bool, uint64_t)> handler){}

void Client::OnDisconnect() {
  timerPing->stop();
  timerReconnect = new Timer();
  timerReconnect->setInterval(Reconnect, 1000);
  timerStopReconnect = new Timer();
  timerStopReconnect->setTimeout(CloseConnexion, 5000);
}

void Client::Disconnect() {
  // Send the DISCONNECT package to the server.
  struct Disconnect package;
  package.uuid = uuid;
  package.size = sizeof(Disconnect);
  std::vector<char> Data = package.serialize();
  stream->SendData(Data);
  CloseConnexion();
}

void Client::Reconnect() {
  // Try to RECONNECT to the server.
  struct Reconnect package;
  package.uuid = uuid;
  package.token = 1;
  package.size = sizeof(Reconnect);
  std::vector<char> Data = package.serialize();
  stream->SendData(Data);
}

void Client::CloseConnexion() {
  // Fermeture de la connexion
  if (timerPing) {
    timerPing->stop();
  }
  if (timerReconnect) {
    timerReconnect->stop();
  }
  delete timerPing;
  delete timerReconnect;
  delete timerStopReconnect;
  delete timerDisconnect;
  stream->Close();
}

void Client::Ping(){
  struct Ping package;
  package.ping_id = ping_id;
  ping_id += 1;
  package.ping_timer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  package.uuid = uuid;
  package.size = sizeof(package);
  std::vector<char> Data = package.serialize();
  stream->SendData(Data);
}

std::unique_ptr<Stream> Client::CreateStream(bool reliable) {
  *stream = Stream(1, false, sockfd, ip_server, port_server, [](std::span<const char>) {
  // SWITCH CASE SUR CE QUON RECOIT
});
}