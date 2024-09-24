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
    virtual void calculateTransmissionParameters() = 0;
    virtual void setEthernetPacket() = 0;
    virtual void transmitPackets(FileHandler *fileHandler) = 0;
    virtual void printTransmissionParams() = 0;
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
    void calculateTransmissionParameters();
    void setEthernetPacket();
    void transmitPackets(FileHandler *fileHandler);
    void printTransmissionParams();
};
class TransmissionHandlerOran : TransmissionHandler
{
    TransmissionConfigOran *transmissionConfigOran;
    EthernetPacketOran *ethernetPacket;

    //Packet Size and Structure Params
    int noOfPacketsPerSymbol;
    int noOfFragmentsPerPacket;
    int noOfIFGsPerFullPacket;
    int noOfIFGsPerLastPacketInFragment;
    int noOfIFGsPerLastPacketInSymbol;
    int maxNRBsPerPacket;
    int lastFragmentNRBs;
    int lastPacketPerSymbolNRBs;
    int noOfFragmentsPerLastPacketInSymbol;
    int lastFragmentInLastPacketPerSymbolNRBs;
    long long noOfSymbols;

    void calculateFrameStructureData();
    void calculateFragmentationRequirements();
    void calculateIFGsPerPacket();

public:
    TransmissionHandlerOran(TransmissionConfigOran *transmissionConfigOran);
    void calculateTransmissionParameters();
    void transmitPackets(FileHandler *fileHandler);
    void printTransmissionParams();
    void setEthernetPacket();
};