#!/bin/bash

# Variables (passées par CMake)
SERVER_EXECUTABLE=$SERVER_EXECUTABLE
CLIENT_EXECUTABLE=$CLIENT_EXECUTABLE

# Lance le serveur en arrière-plan
$SERVER_EXECUTABLE &
SERVER_PID=$!

# Attends un court instant pour s'assurer que le serveur est prêt
sleep 2

# Lance le client
$CLIENT_EXECUTABLE 127.0.0.1 "Albin"
CLIENT_RESULT=$?

# Termine le serveur
kill $SERVER_PID

# Vérifie le résultat
if [ $CLIENT_RESULT -eq 1 ]; then
    echo "Test passed!"
    exit 0
else
    echo "Test failed!"
    exit 1
fi
