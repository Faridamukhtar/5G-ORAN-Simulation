#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "Payload.h"

using namespace std;

string convertToHex(int num, int nChars = 2)
{
    stringstream stream;
    stream << hex << setw(nChars) << setfill('0') << num;
    return stream.str();
}

string signedToHex16Bit(int num)
{
    if (num < 0)
        num += pow(2, 16);

    stringstream ss;
    ss << uppercase << hex << setw(4) << setfill('0') << num;
    return ss.str();
}

int hex16BitToSigned(string hexNum)
{
    int signedNum = stoi(hexNum, nullptr, 16);

    if (signedNum & 0x8000) // checks if negative number by comparing highest bit (2s complement)
        signedNum -= pow(2, 16);

    return signedNum;
}

Payload::Payload(int maxPacketSize, int extraBytesNo)
{
    string dummyData((maxPacketSize - extraBytesNo) * 2, '0');
    payload = dummyData;
}

string Payload::getPayload()
{
    return payload;
}

OranPacket::OranPacket(int RBs, vector<pair<int, int>> IQSamples)
{
    incrementPacket();
    string zeroByte(2, '0');
    firstByte = zeroByte;
    this->RBs = RBs;

    string zeroPoint5Byte(3, '0');
    sectionStartData = zeroPoint5Byte;

    setPayload(IQSamples);
}

void OranPacket::setPayload(vector<pair<int, int>> IQSamples)
{
    payload = "";

    for (auto s : IQSamples)
    {
        string i = signedToHex16Bit(s.first);
        string q = signedToHex16Bit(s.second);
        payload.append(i);
        payload.append(q);
    }
}

int OranPacket::frameId = 0;
int OranPacket::subFrameId = 0;
int OranPacket::slotId = 0;
int OranPacket::symbolId = 0;

int OranPacket::slotsPerSubFrame;
int OranPacket::maxPacketsPerSymbolAfterFragmentation;
int OranPacket::currentPacketsPerSymbol = -1;
int OranPacket::noOfFragmentsPerPacket = 1;
int OranPacket::startRB = 0;

void OranPacket::incrementPacket()
{
    currentPacketsPerSymbol++;
    if (currentPacketsPerSymbol >= maxPacketsPerSymbolAfterFragmentation)
    {
        currentPacketsPerSymbol = 0;
        symbolId++;
        if (symbolId >= 14)
        {
            symbolId = 0;
            slotId++;
            if (slotId >= slotsPerSubFrame)
            {
                slotId = 0;
                subFrameId++;
                if (subFrameId >= 10)
                {
                    subFrameId = 0;
                    frameId++;
                    if (frameId >= 1024)
                    {
                        frameId = 0;
                    }
                }
            }
        }
    }
}

void OranPacket::setMaxPacketsPerSymbolAfterFragmentation(int maxPacketsPerSymbolAfterFragmentation)
{
    OranPacket::maxPacketsPerSymbolAfterFragmentation = maxPacketsPerSymbolAfterFragmentation;
}

void OranPacket::setSlotsPerSubFrame(int slotsPerSubFrame)
{
    OranPacket::slotsPerSubFrame = slotsPerSubFrame;
}

void OranPacket::adjustStartRB()
{
    if (symbolId == 0)
        startRB = 0;
    else
        startRB += RBs;
}

string OranPacket::getOranPacketAsString()
{
    string packet = firstByte + convertToHex(frameId) + convertToHex(subFrameId, 1) + convertToHex(slotId * pow(2, 2)) + convertToHex(symbolId, 1) + sectionStartData + convertToHex(startRB, 3) + convertToHex(RBs, 2) + payload;
    adjustStartRB();
    return packet;
}

EcpriPacket::EcpriPacket(int RBs, vector<pair<int, int>> IQSamples)
{
    string zeroByte(2, '0');
    firstByte = zeroByte;
    message = zeroByte;
    pcRtc = zeroByte + zeroByte;
    payloadSize = RBs * 12 * 4;

    incrementSeqId();
    oranPacket = new OranPacket(RBs, IQSamples);
}

string EcpriPacket::getEcpriPacketAsString()
{

    string packet = firstByte + message + convertToHex(payloadSize, 4) + pcRtc + convertToHex(seqId, 4) + oranPacket->getOranPacketAsString();
    return packet;
}

int EcpriPacket::seqId = -1;

void EcpriPacket::incrementSeqId()
{
    seqId = (seqId + 1) % 255;
}
