//
// Created by Albin Horlaville on 10/02/2025.
//

#ifndef FORMATPACKAGE_H
#define FORMATPACKAGE_H

#include <string>
#include <cstdint>

/* Useless for now
enum Type {
  CONNECT,
  CONNECT_ACK,
  DISCONNECT,
  RECONNECT,
  PING,
  PONG,
  DATA,
  DATA_ACK,
};*/

enum Flag {
  NONE,
  SPLIT
};

// Client -> Server
struct Connect {
  uint16_t size;
  std::string version;
 };

struct Reconnect {
  uint16_t size;
  uint64_t uuid;
  uint64_t token;
 };

struct Disconnect {
  uint16_t size;
  uint64_t uuid;
};

// Server -> Client
struct Connect_ACK {
  uint16_t size;
  uint64_t uuid;
  uint64_t token;
};

// Server <-> Client
struct Ping {
  uint16_t size;
  uint64_t uuid;
  uint64_t ping_id;
  uint64_t ping_timer;
};

struct Data {
  uint16_t size;
  uint64_t uuid;
  uint64_t stream_id; // First bit at 1 = reliable, 0 = unreliable
  Flag flag; // Split ?
  uint64_t part_id; // Useful if split
  uint64_t total; // Useful if split
  uint64_t msg_id;
  std::string data;
};

struct Data_ACK {
  uint16_t size;
  uint64_t uuid;
  uint64_t last_rcv_id;
  uint64_t last_rcv; // State of last 30 packets (0 = not received, 1 = received)
  // If stream is reliable, we send back, else we don't
};

#endif //FORMATPACKAGE_H
