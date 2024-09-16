#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "EthernetPacket.h"

class TransmissionConfig;

using namespace std;

enum FILE_OPERATION
{
    READ_FILE,
    WRITE_FILE
};

class FileHandler
{
    string fileName;
    fstream file;
    void ParseLine(string Line, unordered_map<string, string> &data);

public:
    FileHandler(string fileName, FILE_OPERATION operation);
    ~FileHandler();
    void ReadFile(TransmissionConfig *transmissionConfig);
    void writeFile(string data, int chunkSize = 8);
    void closeFile();
};