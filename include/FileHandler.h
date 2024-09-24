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
    bool ReadNextTCPChunk(string &chunk, int chunkSize = 1024);
    void ReadFile(TransmissionConfig *transmissionConfig);
    bool ReadFileLineByLine(string& line);
    void ReadIQFile(vector<pair<int, int>> &IQSamples, int nLines);
    void writeFile(string data, int chunkSize = 8);
    void writeFileString(string data);
    void closeFile();
};