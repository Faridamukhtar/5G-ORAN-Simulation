
#include "ReceivingHandler.h"

#define BUFFER_SIZE 1024
#define PORT_NUM 65432

using namespace std;

void ReceivingHandler::establishConnection()
{
    // Create a TCP socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        cerr << "Error creating socket" << endl;
        return;
    }

    // Define the server address structure
    struct sockaddr_in server_address, client_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT_NUM);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address
    if (bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        cerr << "Error binding socket" << endl;
        close(server_sock);
        return;
    }

    // Listen for incoming connections
    listen(server_sock, 1);
    cout << "Server is listening on port " << PORT_NUM << "..." << endl;

    socklen_t client_len = sizeof(client_address);
    client_sock = accept(server_sock, (struct sockaddr *)&client_address, &client_len);
    if (client_sock < 0)
    {
        cerr << "Error accepting connection" << endl;
        close(server_sock);
        return;
    }
}

ReceivingHandler::ReceivingHandler(string outputFileName)
{
    outputFileHandler = new FileHandlerRec(outputFileName, WRITE_FILE);
}

void ReceivingHandler::receivePacketsOverTCP()
{
    // Receive data in chunks
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    while ((bytes_received = recv(client_sock, buffer, sizeof(buffer), 0)) > 0)
    {
        outputFileHandler->writeInBytes(buffer, bytes_received);
    }

    if (bytes_received < 0)
    {
        cerr << "Error receiving data" << endl;
    }

    cout << "Data reception complete." << endl;

    delete outputFileHandler;
    close(client_sock);
    close(server_sock);
}

void ReceivingHandler::listenToTCPTransmission()
{
    establishConnection();
    receivePacketsOverTCP();
}