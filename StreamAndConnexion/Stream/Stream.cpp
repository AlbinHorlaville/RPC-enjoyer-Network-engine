//
// Created by Albin Horlaville & Hugo Girard on 10/02/2025.
//

#include "Stream.h"
#include "Socket.h"

Stream::Stream(uint16_t id, bool reliable, std::string ip, uint16_t port_receive, uint16_t port_send){
  this->id = id;
  this->reliable = reliable;
  this->sockfd = Socket::listen(ip, port_receive);
  this->port_receive = port_receive;
  this->port_send = port_send;
  this->ip = ip;
}

bool Stream::isReliable() {
  return this->reliable;
}

uint16_t Stream::getId() {
  return this->id;
}

long Stream::SendData(std::span<const char> Data){
  long size = Socket::sendTo(this->sockfd, this->ip, this->port_send, Data);
  return size;
}

long Stream::ReceiveData(std::span<char, 65535> buffer){
  std::string fake_ip;
  long size = Socket::recvFrom(this->sockfd, fake_ip, buffer);
  return size;
}

void Stream::Close(){
  Socket::close(this->sockfd);
}
