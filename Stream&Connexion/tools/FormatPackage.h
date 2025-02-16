//
// Created by Albin Horlaville on 10/02/2025.
//

#ifndef FORMATPACKAGE_H
#define FORMATPACKAGE_H

#include <string>
#include <cstdint>
#include <span>
#include <vector>

enum Flag {
  NONE,
  SPLIT
};

template<typename T>
void serializeField(std::vector<char>& buffer, const T& value) {
  size_t start = buffer.size();
  buffer.resize(start + sizeof(T));
  std::memcpy(buffer.data() + start, &value, sizeof(T));
}

void serializeString(std::vector<char>& buffer, const std::string& str) {
  uint16_t length = static_cast<uint16_t>(str.size());
  serializeField(buffer, length);
  buffer.insert(buffer.end(), str.begin(), str.end());
}

template<typename T>
void deserializeField(const char*& data, T& value) {
  std::memcpy(&value, data, sizeof(T));
  data += sizeof(T);
}

void deserializeString(const char*& data, std::string& str) {
  uint16_t length;
  deserializeField(data, length);
  str.assign(data, length);
  data += length;
}

// Header : 1, Client -> Server
struct Connect {
  uint16_t size;
  std::string version;

  std::vector<char> serialize() {
    std::vector<char> buffer;
    buffer.push_back(1);
    serializeField(buffer, this->size);
    serializeString(buffer, this->version);
    return buffer;
  }

  void deserialize(const std::span<const char>& buffer) {
    const char* data = buffer.data()+1;
    deserializeField(data, this->size);
    deserializeString(data, this->version);
  }
 };

// Header : 2
struct Reconnect {
  uint16_t size;
  uint64_t uuid;
  uint64_t token;

  std::vector<char> serialize() {
    std::vector<char> buffer;
    buffer.push_back(2);
    serializeField(buffer, this->size);
    serializeField(buffer, this->uuid);
    serializeField(buffer, this->token);
    return buffer;
  }

  void deserialize(const std::span<const char>& buffer) {
    const char* data = buffer.data()+1;
    deserializeField(data, this->size);
    deserializeField(data, this->uuid);
    deserializeField(data, this->token);
  }
 };

// Header : 3
struct Disconnect {
  uint16_t size;
  uint64_t uuid;

  std::vector<char> serialize() {
    std::vector<char> buffer;
    buffer.push_back(3);
    serializeField(buffer, this->size);
    serializeField(buffer, this->uuid);
    return buffer;
  }

  void deserialize(const std::span<const char>& buffer) {
    const char* data = buffer.data()+1;
    deserializeField(data, this->size);
    deserializeField(data, this->uuid);
  }
};

// Header : 4, Server -> Client
struct Connect_ACK {
  uint16_t size;
  uint16_t port;
  uint64_t uuid;
  uint64_t token;

  std::vector<char> serialize() {
    std::vector<char> buffer;
    buffer.push_back(4);
    serializeField(buffer, this->size);
    serializeField(buffer, this->port);
    serializeField(buffer, this->uuid);
    serializeField(buffer, this->token);
    return buffer;
  }

  void deserialize(const std::span<const char>& buffer) {
    const char* data = buffer.data()+1;
    deserializeField(data, this->size);
    deserializeField(data, this->port);
    deserializeField(data, this->uuid);
    deserializeField(data, this->token);
  }
};

// Header : 5, Server <-> Client
struct Ping {
  uint16_t size;
  uint64_t uuid;
  uint64_t ping_id;
  uint64_t ping_timer;

  std::vector<char> serialize() {
    std::vector<char> buffer;
    buffer.push_back(5);
    serializeField(buffer, this->size);
    serializeField(buffer, this->uuid);
    serializeField(buffer, this->ping_id);
    serializeField(buffer, this->ping_timer);
    return buffer;
  }

  void deserialize(const std::span<const char>& buffer) {
    const char* data = buffer.data()+1;
    deserializeField(data, this->size);
    deserializeField(data, this->uuid);
    deserializeField(data, this->ping_id);
    deserializeField(data, this->ping_timer);
  }
};

// Header : 6
struct Data {
  uint16_t size;
  uint64_t uuid;
  uint64_t stream_id; // First bit at 1 = reliable, 0 = unreliable
  Flag flag; // Split ?
  uint64_t part_id; // Useful if split
  uint64_t total; // Useful if split
  uint64_t msg_id;
  std::string data;

  std::vector<char> serialize() {
    std::vector<char> buffer;
    buffer.push_back(6);
    serializeField(buffer, this->size);
    serializeField(buffer, this->uuid);
    serializeField(buffer, this->stream_id);
    serializeField(buffer, this->flag);
    serializeField(buffer, this->part_id);
    serializeField(buffer, this->total);
    serializeField(buffer, this->msg_id);
    serializeString(buffer, this->data);
    return buffer;
  }

  void deserialize(const std::span<const char>& buffer) {
    const char* data = buffer.data()+1;
    deserializeField(data, this->size);
    deserializeField(data, this->uuid);
    deserializeField(data, this->stream_id);
    deserializeField(data, this->flag);
    deserializeField(data, this->part_id);
    deserializeField(data, this->total);
    deserializeField(data, this->msg_id);
    deserializeString(data, this->data);
  }
};

// Header : 7
struct Data_ACK {
  uint16_t size;
  uint64_t uuid;
  uint64_t last_rcv_id;
  uint64_t last_rcv; // State of last 30 packets (0 = not received, 1 = received)
  // If stream is reliable, we send back, else we don't

  std::vector<char> serialize() {
    std::vector<char> buffer;
    buffer.push_back(7);
    serializeField(buffer, this->size);
    serializeField(buffer, this->uuid);
    serializeField(buffer, this->last_rcv_id);
    serializeField(buffer, this->last_rcv);
    return buffer;
  }

  void deserialize(const std::span<const char>& buffer) {
    const char* data = buffer.data()+1;
    deserializeField(data, this->size);
    deserializeField(data, this->uuid);
    deserializeField(data, this->last_rcv_id);
    deserializeField(data, this->last_rcv);
  }
};

inline uint64_t getTimestamp() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

#endif //FORMATPACKAGE_H