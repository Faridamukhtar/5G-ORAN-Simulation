#pragma once
#include <string>
#include "Payload.h"

using namespace std;
class EthernetPacket
{
    const string preamble;
    const string SFD;
    const string destAddress;
    const string srcAddress;
    const string etherType;
    Payload *payload;
    string crc;

public:
    EthernetPacket(int maxPacketSize, string destAddress, string srcAddress, string etherType = "0800", string preamble = "FB555555555555", string SFD = "D5");
    void printProperties();
    void calculateCRC();
    string getPacketAsString();
};