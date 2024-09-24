#pragma once
#include <string>
#include <unordered_map>

#include "ConfigProperties.h"

class TransmissionHandler;
class TransmissionHandlerBurst;
class TransmissionHandlerOran;

extern std::unordered_map<ConfigProperties, std::string> objectPropertyToFilePropertyMapper;

using namespace std;

class TransmissionConfig
{
    friend class TransmissionHandler;

protected:
    double lineRateInGigaBits;
    double captureSizeInMs;
    int minIFGs;
    string destAddress;
    string srcAddress;
    int maxPacketSizeInBytes;
    string remove0xFromAddress(string address);

public:
    virtual void initializeConfigData(unordered_map<string, string> configData);
    virtual void printProperties();
};

class TransmissionConfigBurst : public TransmissionConfig
{
    friend class TransmissionHandlerBurst;
    int burstSize;
    double burstPeriodicityInMicroseconds;
    
public:
    TransmissionConfigBurst();
    void initializeConfigData(unordered_map<string, string> configData);
    void printProperties();
};

class TransmissionConfigOran : public TransmissionConfig
{
    friend class TransmissionHandlerOran;
    int scs;
    int maxNrb;
    int nrbPerPacket;
    string payloadFilename;
    
public:
    TransmissionConfigOran();
    void initializeConfigData(unordered_map<string, string> configData);
    void printProperties();
};