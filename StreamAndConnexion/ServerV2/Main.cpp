//
// Created by Albin Horlaville & Hugo Girard on 10/02/2025.
//

#include <iostream>
#include "Server.h"

int main(int argc, const char * argv[]) {
    Server server = Server();
    std::cout << "Listening..." << std::endl;
    server.Listen();
    while (true) {
        server.Receive();
    }
}