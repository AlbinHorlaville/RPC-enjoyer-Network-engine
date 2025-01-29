#!/bin/bash

# Variables (passées par CMake)
SERVER_EXECUTABLE=$SERVER_EXECUTABLE
CLIENT_EXECUTABLE=$CLIENT_EXECUTABLE

# Vérifie si les exécutables existent et sont exécutables
if [ ! -x "$SERVER_EXECUTABLE" ]; then
    echo "Error: Server executable not found or not executable: $SERVER_EXECUTABLE"
    exit 1
fi

if [ ! -x "$CLIENT_EXECUTABLE" ]; then
    echo "Error: Client executable not found or not executable: $CLIENT_EXECUTABLE"
    exit 1
fi

# Lancer le serveur en arrière-plan
echo "Starting server..."
$SERVER_EXECUTABLE &
SERVER_PID=$!

# Vérifie si le serveur s'est lancé correctement
if ! kill -0 $SERVER_PID 2>/dev/null; then
    echo "Error: Failed to start server."
    exit 1
fi

# Attends un court instant pour s'assurer que le serveur est prêt
sleep 2

# Lance le client
echo "Starting client..."
$CLIENT_EXECUTABLE "127.0.0.1" "Albin"
CLIENT_RESULT=$?

# Arrête proprement le serveur
echo "Stopping server..."
kill $SERVER_PID
wait $SERVER_PID 2>/dev/null

# Vérifie le résultat du client
if [ $CLIENT_RESULT -eq 1 ]; then
    echo "Test passed!"
    exit 0
else
    echo "Test failed!"
    exit 1
fi
