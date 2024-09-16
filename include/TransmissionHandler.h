#pragma once
#include <string>
#include <vector>
#include "EthernetPacket.h"
#include "TransmissionConfig.h"
#include "FileHandler.h"

using namespace std;

class TransmissionHandler
{
public:
    virtual void CalculateTransmissionParameters()=0;
    virtual void initializeEthernetPackets()=0;
    virtual void transmitPackets(FileHandler* fileHandler)=0;
    virtual void printTransmissionParams()=0;
};

class TransmissionHandlerBurst : public TransmissionHandler
{
    TransmissionConfigBurst *transmissionConfigBurst;

    double singleByteDurationInMicroseconds;

    int packetAndIFGsSizeInBytes;
    int IFGsNum;
    double packetAndIFGsDurationInMicroseconds;

    double burstAndPeriodicityDurationInMicroseconds;
    int totalNumberOfCompleteBursts;
    int totalNumberOfIFGsBetweenBursts;

    int numberOfLastBurstPackets;
    int numberOfIFGsAfterLastPacket;

    EthernetPacket *ethernetPacket; // Only a single packet needed because in this mode, all packets are identical

    void calculatePacketSize();
    void calculatePacketDuration();
    void calculateTotalNumberOfCompleteBursts();
    void calculateNumberOfLastBurstPackets();
    void calculateNumberOfIFGsAfterLastPacket();
    void calculateSingleByteDurationInMicroseconds();

public:
    TransmissionHandlerBurst(TransmissionConfigBurst *transmissionConfigBurst);
    ~TransmissionHandlerBurst();
    void CalculateTransmissionParameters();
    void initializeEthernetPackets();
    void transmitPackets(FileHandler* fileHandler);
    void printTransmissionParams();
};