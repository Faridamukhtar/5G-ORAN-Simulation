#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

enum FILE_OPERATION
{
    READ_FILE,
    WRITE_FILE
};

class FileHandlerRec
{
    string fileName;
    fstream file;

public:
    FileHandlerRec(string fileName, FILE_OPERATION operation);
    ~FileHandlerRec();
    bool ReadNextTCPChunk(string &chunk, int chunkSize = 1024);
    bool ReadFileLineByLine(string& line);
    void writeFile(string data, int chunkSize = 8);
    void writeFileString(string data);
    void closeFile();
    void writeInBytes(char* buffer, ssize_t bytes_received);
};