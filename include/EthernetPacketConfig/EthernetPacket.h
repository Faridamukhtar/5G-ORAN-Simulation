#pragma once
#include <string>
#include "Payload.h"

using namespace std;

class EthernetCRC
{
};
class EthernetPacket
{
    string preamble;
    string SFD;
    string destAddress;
    string srcAddress;
    Payload payload;
    EthernetCRC crc;
};