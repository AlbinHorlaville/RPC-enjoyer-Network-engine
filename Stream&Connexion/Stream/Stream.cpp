//
// Created by Albin Horlaville on 10/02/2025.
//

#include "Stream.h"
#include "Socket.h"

Stream::Stream(uint16_t id, bool reliable, int sockfd, std::string ip, uint16_t port, std::function<void(std::span<const char>)> handler){
  this->id = id;
  this->reliable = reliable;
  this->sockfd = sockfd;
  this->port = port;
  this->ip = ip;
  this->onDataReceivedHandler = std::move(handler);
}

bool Stream::isReliable() {
  return this->reliable;
}

uint16_t Stream::getId() {
  return this->id;
}

long Stream::SendData(std::span<const char> Data){
  long size = Socket::sendTo(this->sockfd, this->ip, this->port, Data);
  return size;
}

long Stream::ReceiveData(std::span<char, 65535> buffer){
  long size = Socket::recvFrom(this->sockfd, this->ip, buffer);
  OnDataReceived(buffer);
  return size;
}

void Stream::OnDataReceived(std::span<const char> Data){
  this->onDataReceivedHandler(Data);
}

void Stream::Close(){
  Socket::close(this->sockfd);
}
