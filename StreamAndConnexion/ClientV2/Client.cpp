//
// Created by Albin Horlaville on 10/02/2025.
//

#include "Client.h"
#include "FormatPackage.h"
#include "Socket.h"

#define PORT_SEND 5555
#define PORT_RECEIVE 5556

Client::Client() {
  ping_id = 0;
  uuid = 0;
  token = 0;
  msg_id = 0;
  sockfd = 0;
  ip_server = "";
  port_server = 0;
  version = "1.0.0";
  latence = 0;
  timerDisconnect = std::make_unique<Timer>();
  timerPing = std::make_unique<Timer>();
  timerReconnect = std::make_unique<Timer>();
  timerStopReconnect = std::make_unique<Timer>();
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
  std::cout << "Ping " << package.ping_id << std::endl;
}

void Client::ConnectTo(const std::string& ip){
  Socket::initialize();
  sockfd = Socket::connect(ip, PORT_RECEIVE);

  // Send the package CONNECT
  struct Connect package;
  package.version = version;
  package.size = sizeof(package);
  std::vector<char> Data = package.serialize();
  ip_server = ip;
  Socket::sendTo(sockfd, ip, PORT_SEND, Data);

  std::array<char, 65535> buffer;

  std::string ip_client;

  // Wait until we receive a CONNECT or a RECONNECT package
  long long read_bytes = Socket::recvFrom(sockfd, ip_client, std::span<char, 65535>(buffer));
  if (read_bytes < 0) {
    Socket::close(sockfd);
    Socket::cleanup();
    exit(EXIT_FAILURE);
  }

  // Deserialize Package
  struct Connect_ACK connect_ack{};
  connect_ack.deserialize(buffer);
  uuid = connect_ack.uuid;
  token = connect_ack.token;
  port_server = connect_ack.port;

  // Create Stream
  stream = std::make_unique<Stream>(1, false, ip, port_server, port_server+1);

  // Start the PING timer
  timerPing->setInterval([this](){Ping();}, 200);

  // Start the DISCONNECT timer
  timerDisconnect->setTimeout([this](){OnDisconnect();}, 1000);
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

void Client::CloseConnexion() {
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
  // Try to RECONNECT to the server.
  struct Reconnect package;
  package.uuid = uuid;
  package.token = token;
  package.size = sizeof(package);
  std::vector<char> Data = package.serialize();
  Socket::sendTo(sockfd, ip_server, PORT_SEND, Data);
  stream->SendData(Data);
}

void Client::CreateStream(bool reliable) {
  *stream = Stream(1, reliable, ip_server, PORT_RECEIVE, port_server);
}

uint16_t Client::GetLatence() const {
  return latence;
}

void Client::OnDisconnect() {
  stream->Close();
  timerPing->stop();
  timerDisconnect->stop();
  timerReconnect->setInterval([this](){Reconnect();}, 1000);
  timerStopReconnect->setTimeout([this](){CloseConnexion();}, 5000);
}

void Client::ReceiveData() {
  std::array<char, 65535> bufferReceive;
  std::vector<char> bufferSend;
  stream->ReceiveData(std::span<char, 65535>(bufferReceive));

  switch (bufferReceive[0]) {
    case 5: {
      // PONG
      // Deserialize Package
      struct Ping pong{};
      pong.deserialize(bufferReceive);

      // Calcul the latence
      latence = getTimestamp() - pong.ping_timer;

      // Reset the timer
      timerDisconnect->stop();
      timerDisconnect->setTimeout([this](){OnDisconnect();}, 1000);
      std::cout << "Pong " << pong.ping_id << std::endl;
      break;
    }
    case 6: {
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
      data_ack.last_rcv_id = data.msg_id;
      data_ack.last_rcv = 0;
      data_ack.size = sizeof(data_ack);
      bufferSend = data_ack.serialize();
      stream->SendData(bufferSend);
      break;
    }
    case 7: {
      // DATA ACK
      // Deserialize Package
      struct Data_ACK ack{};
      ack.deserialize(bufferReceive);
      std::cout << "DATA_ACK received : " << ack.last_rcv_id << std::endl;
      // A finir : Faire un suivi des messages envoyer pour que l'ack ait un intérêt.
      break;
    }
    default: {
      // Never happen
      break;
    }
  }
}
