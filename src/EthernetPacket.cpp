#include <math.h>
#include <iostream>
#include <zlib.h>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "EthernetPacket.h"

using namespace std;

EthernetPacket::EthernetPacket(int maxPacketSize, string destAddress, string srcAddress,string etherType, string preamble, string SFD) : preamble(preamble), SFD(SFD), destAddress(destAddress), srcAddress(srcAddress), etherType(etherType)
{
    payload = new Payload(maxPacketSize);
    calculateCRC();
}

void EthernetPacket::printProperties()
{
    cout << "Preamble: " << preamble << endl;
    cout << "SFD: " << SFD << endl;
    cout << "Destination Address: " << destAddress << endl;
    cout << "Source Address: " << srcAddress << endl;
    cout << "EtherType: " << etherType << endl;
    cout << "CRC: " << crc << endl;

    //TODO: PRINT PAYLOAD HERE IF NEEDED
}

string EthernetPacket::getPacketAsString()
{
    return preamble + SFD + destAddress + srcAddress + etherType + payload->getPayload() + crc;
}

void EthernetPacket::calculateCRC()
{
    unsigned long crcInit = crc32(0L, Z_NULL, 0);
    string packetData = destAddress + srcAddress + payload->getPayload();
    uLong crcDecimal = crc32(crcInit, reinterpret_cast<const Bytef*>(packetData.c_str()), packetData.size());
    stringstream stream;
    stream << hex << setw(8) << setfill('0') << crcDecimal;
    crc = stream.str();
}
