#pragma once
#include <string>
#include <unordered_map>

#include "EthernetPacket.h"
#include"fileToObjectMapper.h"

using namespace std;

class TransmissionConfig
{
protected:
    double lineRateInGigaBits;
    double captureSizeInMs;
    int minIFGs;
    string destAddress;
    string srcAddress;
    int maxPacketSizeInBytes;

public:
    virtual void initializeConfigData(unordered_map<string, string> configData) = 0;
};

class TransmissionConfigBurst : public TransmissionConfig
{
    int burstSize;
    double burstPeriodicityInMicroseconds;

public:
    TransmissionConfigBurst();
    void initializeConfigData(unordered_map<string, string> configData);
    void printProperties();
};