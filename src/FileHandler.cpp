#include <iostream>
#include <unordered_map>

#include "FileHandler.h"
#include "TransmissionConfig.h"

using namespace std;

FileHandler::FileHandler(string fileName, FILE_OPERATION operation)
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

FileHandler::~FileHandler()
{
    closeFile();
}

bool FileHandler::ReadNextTCPChunk(string &chunk, int chunkSize)
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

void FileHandler::ReadFile(TransmissionConfig *TransmissionConfig)
{
    string line;
    unordered_map<string, string> data;

    while (getline(file, line))
    {
        ParseLine(line, data);
    }
    TransmissionConfig->initializeConfigData(data);
}

bool FileHandler::ReadFileLineByLine(string &line)
{
    if (!file.eof())
    {
        getline(file, line);
        return true;
    }

    return false;
}

void FileHandler::ReadIQFile(vector<pair<int, int>> &IQSamples, int nLines)
{
    IQSamples.clear();

    int i, q;

    for (int j = 0; j < nLines; j++)
    {
        if (file.eof())
        {
            file.clear();
            file.seekg(0, ios::beg);
        }
        file >> i >> q;
        IQSamples.emplace_back(i, q);
    }
}

void FileHandler::writeFile(string data, int chunkSize)
{
    // prints each 4 bytes in 1 line
    for (size_t i = 0; i < data.length(); i += chunkSize)
    {
        file << data.substr(i, chunkSize) << endl;
    }
}

void FileHandler::writeFileString(string data)
{
    file << data;
    file.flush();
}

void FileHandler::closeFile()
{
    file.close();
}

void FileHandler::ParseLine(string line, unordered_map<string, string> &data)
{
    size_t pos = line.find('=');
    if (pos != string::npos)
    {
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);

        key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);
        value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));

        data[key] = value;
    }
    else
    {
        cerr << "Warning: Line does not contain '=' character: " << line << endl;
    }
}