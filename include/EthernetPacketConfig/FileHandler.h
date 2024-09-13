#pragma once
#include <string>
#include <fstream>
#include <iostream>

#include"EthernetPacket.h"

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
    void ParseLine(string Line, unordered_map<string, string>& data);
public:
    FileHandler(string fileName, FILE_OPERATION operation);
    void ReadFile(TransmissionConfig *transmissionConfig);
    //void writeFile(vector<EthernetPacket> packets);
};