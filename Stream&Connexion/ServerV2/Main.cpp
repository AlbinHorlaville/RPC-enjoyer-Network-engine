//
// Created by Albin Horlaville on 10/02/2025.
//
#include <iostream>

#include "Server.h"

int main(int argc, const char * argv[]) {
    Server server = Server();
    std::cout << "Listening..." << std::endl;
    server.Listen();
    std::cout << "Client connected!" << std::endl;
    server.Receive();
    server.SendData(server.GetTestClientID(), "Le Québec est une province du Canada dont les frontières correspondent au territoire de la nation québécoise. Vivent également au sein du territoire québécois onze nations autochtones. La capitale du Québec est la ville de Québec, et sa métropole est Montréal. La langue officielle du Québec est le français.");
    server.Receive();
    server.Receive();
}