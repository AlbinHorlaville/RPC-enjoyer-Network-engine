//
// Created by Albin Horlaville on 10/02/2025.
//

#ifndef FORMATPACKAGE_H
#define FORMATPACKAGE_H

#include <string>
#include <cstdint>

enum Type {
  CONNECT,
  CONNECT_ACK,
  DISCONNECT,
  RECONNECT,
  PING,
  PONG,
  DATA,
  DATA_ACK,
};

enum Flag {
  NONE,
  SPLIT
};

struct Package {
  Type type;
  uint16_t size;
  std::string version;
  uint64_t uuid;
  uint64_t token;
  uint64_t ping_id;
  uint64_t ping_timer;
  uint64_t stream_id;
  Flag flag;
  uint64_t part_id; // Si c'est split
  uint64_t total; // Si c'est split
  uint64_t msg_id;
  std::string data;
  uint64_t last_rcv_id;
  uint64_t last_rcv; // 30 derniers paquets reçu
};

#endif //FORMATPACKAGE_H
