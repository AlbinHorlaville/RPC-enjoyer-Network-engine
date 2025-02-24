//
// Created by Albin Horlaville & Hugo Girard on 10/02/2025.
//

#include "Client.h"
#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    Client client;
    client.ConnectTo("127.0.0.1");

    client.SendData("Bien le bonjour au serveur !");

    for (int i = 0; i < 10; i++) {
        client.SendData("Bien le bonjour au serveur !");
        // Receive Send ACK
        client.ReceiveData();
    }


    cout << "Latence : " << client.GetLatence() << "ms." << endl;

    client.Disconnect();

    client.Reconnect();

    while (true) {
        client.ReceiveData();
    }
}
