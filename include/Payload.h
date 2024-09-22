#pragma once
#include <string>

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
    int frameId;
    int subFrameId;
    int slotId;
    int symbolId;
    string sectionsPadding;
public:
    OranPacket();
};

class EcpriPacket
{
    string firstByte;
    string message;
    int payloadSize;
    string pcRtc;
    int seqId;
    OranPacket* oranPacket;
public:
    EcpriPacket();
};