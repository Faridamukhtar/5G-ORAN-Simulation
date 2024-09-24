#pragma once
#include <string>
#include "Payload.h"

using namespace std;

class EthernetPacketTemplate
{
protected:
    const string preamble;
    const string SFD;
    const string destAddress;
    const string srcAddress;
    const string etherType;
    string crc;
    string packetDataWithoutCRC;

public:
    EthernetPacketTemplate(string destAddress, string srcAddress, string etherType = "0800", string preamble = "FB555555555555", string SFD = "D5");
    void printProperties();
    void calculateCRC();
    string getPacketAsString();
    virtual void setPacketDataWithoutCRC() = 0;
    virtual ~EthernetPacketTemplate() = default;
};
class EthernetPacket : public EthernetPacketTemplate
{
    Payload* payload;
public:
    EthernetPacket(int maxPacketSize, string destAddress, string srcAddress, string etherType = "0800", string preamble = "FB555555555555", string SFD = "D5");
    void setPacketDataWithoutCRC();
};

class EthernetPacketOran : public EthernetPacketTemplate
{
    EcpriPacket* ecpriPacket;
public:
    EthernetPacketOran(int RBs, vector<pair<int, int>> IQSamples, string destAddress, string srcAddress, string etherType = "0800", string preamble = "FB555555555555", string SFD = "D5");
    void setPacketDataWithoutCRC();
};
