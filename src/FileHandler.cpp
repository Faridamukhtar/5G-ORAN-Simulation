#include <string>
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