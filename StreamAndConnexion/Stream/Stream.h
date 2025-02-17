//
// Created by Albin Horlaville & Hugo Girard on 10/02/2025.
//

#ifndef STREAM_H
#define STREAM_H

#include <cstdint>
#include <span>
#include <string>

class Stream {
    private:
        uint16_t id;
        bool reliable;
        int sockfd;
        std::string ip;
        uint16_t port_receive;
        uint16_t port_send;

    public:
        Stream(uint16_t id, bool reliable, std::string ip, uint16_t port_receive, uint16_t port_send);
        long SendData(std::span<const char> Data);
        long ReceiveData(std::span<char, 65535> buffer);
        bool isReliable();
        uint16_t getId();
        void Close();
};



#endif //STREAM_H
