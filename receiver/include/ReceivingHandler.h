#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>

#include "FileHandlerRec.h"

using namespace std;

class ReceivingHandler 
{
    FileHandlerRec* outputFileHandler;
    void establishConnection();
    void receivePacketsOverTCP();
    int client_sock;
    int server_sock;
public:
    ReceivingHandler(string outputFileName);
    void listenToTCPTransmission();
};