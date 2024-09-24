#include <math.h>
#include <zlib.h>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "EthernetPacket.h"

using namespace std;

EthernetPacketTemplate::EthernetPacketTemplate(string destAddress, string srcAddress, string etherType, string preamble, string SFD) : preamble(preamble), SFD(SFD), destAddress(destAddress), srcAddress(srcAddress), etherType(etherType) {}

void EthernetPacketTemplate::printProperties()
{
    cout << "Preamble: " << preamble << endl;
    cout << "SFD: " << SFD << endl;
    cout << "Destination Address: " << destAddress << endl;
    cout << "Source Address: " << srcAddress << endl;
    cout << "EtherType: " << etherType << endl;
    cout << "CRC: " << crc << endl;
}

void EthernetPacketTemplate::calculateCRC()
{
    unsigned long crcInit = crc32(0L, Z_NULL, 0);
    uLong crcDecimal = crc32(crcInit, reinterpret_cast<const Bytef *>(packetDataWithoutCRC.c_str()), packetDataWithoutCRC.size());
    stringstream stream;
    stream << hex << setw(8) << setfill('0') << crcDecimal;
    crc = stream.str();
}

string EthernetPacketTemplate::getPacketAsString()
{
    return packetDataWithoutCRC + crc;
}

EthernetPacket::EthernetPacket(int maxPacketSize, string destAddress, string srcAddress, string etherType, string preamble, string SFD) : EthernetPacketTemplate(destAddress, srcAddress, etherType, preamble, SFD)
{
    payload = new Payload(maxPacketSize);
    setPacketDataWithoutCRC();
    calculateCRC();
}

void EthernetPacket::setPacketDataWithoutCRC()
{
    packetDataWithoutCRC = preamble + SFD + destAddress + srcAddress + etherType + payload->getPayload();
}

EthernetPacketOran::EthernetPacketOran(int RBs, vector<pair<int, int>> IQSamples, string destAddress, string srcAddress, string etherType, string preamble, string SFD) : EthernetPacketTemplate(destAddress, srcAddress, etherType, preamble, SFD)
{
    ecpriPacket = new EcpriPacket(RBs, IQSamples);
    setPacketDataWithoutCRC();
    calculateCRC();
}

void EthernetPacketOran::setPacketDataWithoutCRC()
{
    packetDataWithoutCRC = preamble + SFD + destAddress + srcAddress + etherType + ecpriPacket->getEcpriPacketAsString();
}
