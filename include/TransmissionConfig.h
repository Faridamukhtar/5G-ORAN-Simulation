#pragma once
#include <string>
#include <unordered_map>

#include "ConfigProperties.h"

class TransmissionHandler;
class TransmissionHandlerBurst;

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

public:
    virtual void initializeConfigData(unordered_map<string, string> configData) = 0;
};

class TransmissionConfigBurst : public TransmissionConfig
{
    friend class TransmissionHandlerBurst;
    int burstSize;
    double burstPeriodicityInMicroseconds;
    
    string remove0xFromAddress(string address);

public:
    TransmissionConfigBurst();
    void initializeConfigData(unordered_map<string, string> configData);
    void printProperties();
};