#include <iostream>
#include <unordered_map>

#include "FileHandlerRec.h"

using namespace std;

FileHandlerRec::FileHandlerRec(string fileName, FILE_OPERATION operation)
{
    this->fileName = fileName;
    while (!file.is_open())
    {
        if (operation == READ_FILE)
        {
            file.open(fileName, ios::in);
        }
        else
        {
            file.open(fileName, ios::out);
        }
    }
}

FileHandlerRec::~FileHandlerRec()
{
    closeFile();
}

bool FileHandlerRec::ReadNextTCPChunk(string &chunk, int chunkSize)
{
    if (!file.is_open() || file.eof())
    {
        return false;
    }

    char buffer[chunkSize];

    file.read(buffer, chunkSize);
    streamsize bytes_read = file.gcount();

    if (bytes_read > 0)
    {
        chunk.assign(buffer, bytes_read);
        return true;
    }

    return false;
}

bool FileHandlerRec::ReadFileLineByLine(string &line)
{
    if (!file.eof())
    {
        getline(file, line);
        return true;
    }

    return false;
}

void FileHandlerRec::writeFile(string data, int chunkSize)
{
    // prints each 4 bytes in 1 line
    for (size_t i = 0; i < data.length(); i += chunkSize)
    {
        file << data.substr(i, chunkSize) << endl;
    }
}

void FileHandlerRec::writeFileString(string data)
{
    file << data;
    file.flush();
}

void FileHandlerRec::closeFile()
{
    file.close();
}

void FileHandlerRec::writeInBytes(char *buffer, ssize_t bytes_received)
{
    file.write(buffer, bytes_received);
}
