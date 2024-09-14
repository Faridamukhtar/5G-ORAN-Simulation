#pragma once
#include <string>
#include "Payload.h"

using namespace std;

class EthernetCRC
{
};
class EthernetPacket
{
    const string preamble;
    const string SFD;
    const string destAddress;
    const string srcAddress;
    const string etherType;
    Payload* payload;
    EthernetCRC crc;
public:
    EthernetPacket(int maxPacketSize, string destAddress, string srcAddress, string etherType, string preamble, string SFD);
};

class EthernetPacketWrapper
{
    EthernetPacket ethernetPacket;
    int IFGsNum;
public:
    EthernetPacketWrapper(int minNoOfIFGs, int maxPacketSize, string destAddress, string srcAddress, string etherType, string preamble, string SFD);
};