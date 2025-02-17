//
// Created by Albin Horlaville on 10/02/2025.
//

#include "Client.h"
#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    Client client;
    client.ConnectTo("127.0.0.1");

    client.SendData("Bien le bonjour au serveur !");

    // Receive Send ACK
    client.ReceiveData();

    client.ReceiveData();

    cout << "Latence : " << client.GetLatence() << endl;

    //client.Disconnect();
}
