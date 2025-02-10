//
// Created by Albin Horlaville on 10/02/2025.
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
        uint16_t port;

        std::function<void(std::span<const char>)> onDataReceivedHandler;

    public:

        Stream(uint16_t id, bool reliable, int sockfd, std::string ip, uint16_t port, std::function<void(std::span<const char>)> handler);
        long SendData(std::span<const char> Data);
        long ReceiveData(std::span<char, 65535> buffer);
        void OnDataReceived(std::span<const char> Data);
        void Close();
};



#endif //STREAM_H
