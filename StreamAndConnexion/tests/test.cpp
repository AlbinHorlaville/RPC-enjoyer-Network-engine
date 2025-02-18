//
// Created by Albin Horlaville on 18/02/2025.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <thread>

#include "Client.h"
#include "FormatPackage.h"
#include "Server.h"

TEST_CASE("Client connects to the server", "[Client]") {
    Client client;
    Server server;

    std::thread serverThread([&server]() {
        REQUIRE_NOTHROW(server.Listen());
    });

    REQUIRE_NOTHROW(client.ConnectTo("127.0.0.1"));

    serverThread.detach();
}

TEST_CASE("Client send and receive a message", "[Client]") {
    Client client;
    Server server;

    std::thread serverThread([&server]() {
        REQUIRE_NOTHROW(server.Listen());
    });
    REQUIRE_NOTHROW(client.ConnectTo("127.0.0.1"));

    REQUIRE_NOTHROW(client.SendData("Hello Server!"));

    int package;
    int count = 0;
    do {
        package = server.Receive();
        count++;
    }while(package != DATA || count > 100000);
    REQUIRE(count < 100000);

    count = 0;
    do {
        package = client.ReceiveData();
        count++;
    }while(package != DATA_ACK || count > 100000);
    REQUIRE(count < 100000);

    serverThread.detach();
}

TEST_CASE("Client Ping, Server Pong.", "[Client]") {
    Client client;
    Server server;

    std::thread serverThread([&server]() {
        REQUIRE_NOTHROW(server.Listen());
    });
    REQUIRE_NOTHROW(client.ConnectTo("127.0.0.1"));

    int package;
    int count = 0;
    do {
        package = server.Receive();
        count++;
    }while(package != PING || count > 100000);
    REQUIRE(count < 100000);

    count = 0;
    do {
        package = client.ReceiveData();
        count++;
    }while(package != PING || count > 100000);
    REQUIRE(count < 100000);

    serverThread.detach();
}
