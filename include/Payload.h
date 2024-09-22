#pragma once
#include <string>
#include <vector>

#define ORAN_HEADER_SIZE 16
#define ECPRI_HEADER_SIZE 8
#define ETHERNET_HEADER_SIZE 26

using namespace std;

class Payload
{
    string payload;

public:
    Payload(int maxPacketSize, int extraBytesNo = 26);
    string getPayload();
};

class OranPacket
{
    string firstByte;
    int RBs;
    static int frameId;
    static int subFrameId;
    static int slotId;
    static int symbolId;
    static int startRB;
    string sectionStartData;
    string payload;

    static int slotsPerSubFrame;
    static int maxPacketsPerSymbolAfterFragmentation;
    static int currentPacketsPerSymbol;
    static int noOfFragmentsPerPacket;

public:
    OranPacket(int RBs, vector<pair<int,int>> IQSamples);
    void setPayload(vector<pair<int, int>> IQSamples);
    static void incrementPacket();
    static void setMaxPacketsPerSymbolAfterFragmentation(int maxPacketsPerSymbolAfterFragmentation);
    static void setSlotsPerSubFrame(int slotsPerSubFrame);
    void adjustStartRB();
    string getOranPacketAsString();
};

class EcpriPacket
{
    static int seqId;
    string firstByte;
    string message;
    int payloadSize;
    string pcRtc;
    OranPacket *oranPacket;

    static void incrementSeqId();

public:
    EcpriPacket(int RBs, vector<pair<int, int>> IQSamples);
    string getEcpriPacketAsString();
};