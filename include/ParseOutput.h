#pragma once
#include <string>
#include <utility>
#include <unordered_map>
#include <iostream>

#include "nlohmannJson.hpp"
#include "FileHandler.h"

using namespace std;

using json = nlohmann::json;

struct ParsedPacket {
    // Ethernet fields
    string destAddress;
    string srcAddress;
    string etherType;
    
    // eCPRI fields
    string ecpriVersionReservedConcat;
    string ecpriMessage;
    string ecpriPayload;
    string ecpriPcRtc;
    string ecpriSeqId;
    
    // ORAN fields
    string oranDataDirectionPLVersionFilterIndex;
    string oranFrameId;
    string oranSubFrameId;
    string oranSlotId;
    string oranSymbolId;
    string sectionIdRbSymInc;
    string startPrbu;
    string numPrbu;
    string crc;
    
    // Payload
    vector<pair<int, int>> IQSamples;

    // Functions to serialize the packet data as JSON
    json printVectorInJson();
    json toJsonObject();

    void printProperties();
};

class ParseOutput
{
    FileHandler* fileHandlerWrite;
public:
    ParseOutput(string outputFilename);
    ~ParseOutput();
    void parseOutput(string outputFilename);
};